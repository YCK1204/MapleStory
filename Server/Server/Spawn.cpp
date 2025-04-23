#include "pch.h"
#include "PacketHandler.h"
#include "functional"

// [ 패킷 크기 ushort 2byte][ 패킷 종류 ushort  ] [uint64][uint64][ushort]

void PacketHandler::C_DespawnHandler(PacketSession* session, ByteRef& buffer)
{
	// 플레이어 정보 저장 코드 추가 필요

	ClientSession* client = reinterpret_cast<ClientSession*>(session);

	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}
	GameRoomRef room = client->Player->Room;
	room->PushJob(bind(&GameRoom::RemovePlayer, client->Player->Room.get(), client->Player->Id));
}

void PacketHandler::C_PortalHandler(PacketSession* session, ByteRef& buffer)
{
	ClientSession* client = reinterpret_cast<ClientSession*>(session);

	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}

	auto pkt = GetRoot<C_Portal>(buffer->operator std::byte * ());

	try {
		PlayerRef player = client->Player;
		auto portalId = pkt->portal_id();

		if (player->Room->CanPort(portalId) == false)
		{
			client->Disconnect();
			return;
		}

		auto portal = Manager::Data.FindPortal(portalId);
		if (portal == nullptr)
		{
			client->Disconnect();
			return;
		}

		auto targetPortal = Manager::Data.FindPortal(portal->TargetId);
		auto server = Manager::Server.Find(client->ServerId);
		auto channel = server->FindChannel(client->ChannelId);
		auto room = channel->FindRoom(targetPortal->SceneId);

		room->PushJob([room, client, targetPortal]() {
			auto player = client->Player;
			player->Room->RemovePlayer(player->Id);
			room->Push(player);
			player->SetMapId(targetPortal->SceneId);
			player->Room = room;
			player->Pos->X = targetPortal->X;
			player->Pos->Y = targetPortal->Y;

			FlatBufferBuilder builder;
			auto myPlayerInfo = player->GenerateInfoDetail(builder);
			auto position = player->GeneratePosition(builder);
			auto data = CreateSC_Portal(builder, targetPortal->SceneId, myPlayerInfo, position);
			auto pacekt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_Portal);
			client->Send(pacekt);
			});
	}
	catch (...)
	{

	}
}

void PacketHandler::C_EnterGameHandler(PacketSession* session, ByteRef& buffer)
{
	auto client = Manager::Session.Find(session->GetSessionId());

	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}
	auto pkt = GetRoot<C_EnterGame>(buffer->operator std::byte * ());

	try {
		PlayerRef player = client->Player;
		auto mapId = pkt->map_id();
		if (player->GetMapId() != mapId)
		{
			client->Disconnect();
			return;
		}
		auto server = Manager::Server.Find(client->ServerId);
		auto channel = server->FindChannel(client->ChannelId);
		auto room = channel->FindRoom(mapId);

		room->PushJob<GameRoomRef, PlayerRef, ClientRef>([room, player, client](GameRoomRef, PlayerRef, ClientRef) {
			player->Pos->X = room->InitPos[0];
			player->Pos->Y = room->InitPos[1];
			// 입장하려는 유저에게 자신의 캐릭터 정보를 알림
			player->Room = room;
			room->Push(player);
			FlatBufferBuilder builder;
			auto myPlayerInfo = player->GenerateInfoDetail(builder);
			auto position = player->GeneratePosition(builder);
			auto data = CreateSC_EnterGame(builder, player->GetMapId(), myPlayerInfo, position);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_EnterGame);
			client->Send(packet);
			}, room, player, client);
	}
	catch (...)
	{

	}
}

void PacketHandler::C_CreatureInfosHandler(PacketSession* session, ByteRef& buffer)
{
	try {
		ClientRef client = Manager::Session.Find(session->GetSessionId());
		if (client->State != ClientState::INGAME)
		{
			client->Disconnect();
			return;
		}

		PlayerRef player = client->Player;
		GameRoomRef room = player->Room;

		room->PushJob([room, client]() {
			FlatBufferBuilder builder;
			auto playerInfos = room->GenPlayerInfos(builder);
			auto data = CreateSC_CreatureInfos(builder, playerInfos);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CreatureInfos);
			client->Send(pkt);
			});
	}
	catch (...)
	{

	}
}
