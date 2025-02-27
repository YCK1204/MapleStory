#include "pch.h"
#include "PacketHandler.h"
#include "functional"

// [ 패킷 크기 ushort 2byte][ 패킷 종류 ushort  ] [uint64][uint64][ushort]

void PacketHandler::C_DespawnHandler(PacketSession* session, ByteRef& buffer)
{
	// 플레이어 정보 저장 코드 추가 필요

	ClientRef client = Manager::Session.Find(session->GetSessionId());
	if (client == nullptr || client->Player == nullptr)
	{
		session->Disconnect();
		return;
	}
	GameRoomRef room = client->Player->Room;
	if (room == nullptr)
		return;

	FlatBufferBuilder builder;

	auto data = CreateSC_Despawn(builder, client->Player->GetCharId()); // <- 이 코드 절대 레전드 버그남 수정해야함 ////////
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_Despawn);

	room->Remove(client->Player->Id);
	room->Broadcast(packet);
}

void PacketHandler::C_EnterMapHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<C_EnterMap>(buffer->operator std::byte * ());

	try {
		ClientRef client = Manager::Session.Find(session->GetSessionId());
		if (client == nullptr)
		{
			session->Disconnect();
			return;
		}
		PlayerRef player = client->Player;
		auto mapId = pkt->map_id();
		//if (player == nullptr || player->CurMapId == -1 || player->CurMapId != mapId)
		if (player == nullptr || player->Room == nullptr) // 룸끼리 이동할 수 있는지 유효성 검사 필요
		{
			client->Disconnect();
			return;
		}
		auto server = Manager::Server.Find(client->ServerId);
		if (server == nullptr)
		{
			client->Disconnect();
			return;
		}
		auto channel = server->FindChannel(client->ChannelId);
		if (channel == nullptr)
		{
			client->Disconnect();
			return;
		}
		auto room = channel->FindRoom(mapId);
		if (room == nullptr)
		{
			client->Disconnect();
			return;
		}

		{
			auto b = std::bind(static_cast<void(GameRoom::*)(PlayerRef)>(&GameRoom::Remove), player->Room, std::placeholders::_1);
			player->Room->PushJob<PlayerRef&>(b, player);
		}
		player->Room = room;
		{
			auto b = std::bind(static_cast<void(GameRoom::*)(PlayerRef)>(&GameRoom::Push), room, std::placeholders::_1);
			room->PushJob<PlayerRef>(b, player);
		}
		{
			FlatBufferBuilder builder;
			auto playerInfo = player->GeneratePlayerInfo(builder);

			auto data = CreateSC_PSpawn(builder, playerInfo);
			auto playerDataPkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_PSpawn);

			auto b = std::bind(&GameRoom::Broadcast, room, std::placeholders::_1, std::placeholders::_2);
			room->PushJob<SendBufferRef, PlayerRef>(b, playerDataPkt, nullptr);
		}

		// 입장하려는 유저에게 룸에있는 유저들의 정보를 알림
		{
			FlatBufferBuilder builder;
			auto myPlayerInfo = player->GenerateTotalInfo(builder);
			auto data = CreateSC_EnterMap(builder, player->GetMapId(), myPlayerInfo);
			auto pacekt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_EnterMap);
			client->Send(pacekt);
		}
	}
	catch (...)
	{

	}
}

void PacketHandler::C_EnterGameHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<C_EnterGame>(buffer->operator std::byte * ());

	try {
		ClientRef client = Manager::Session.Find(session->GetSessionId());
		if (client == nullptr)
		{
			session->Disconnect();
			return;
		}
		PlayerRef player = client->Player;
		auto mapId = pkt->map_id();
		if (player == nullptr || player->GetMapId() == -1 || player->GetMapId() != mapId || player->Room != nullptr)
		{
			client->Disconnect();
			return;
		}
		auto server = Manager::Server.Find(client->ServerId);
		if (server == nullptr)
		{
			client->Disconnect();
			return;
		}
		auto channel = server->FindChannel(client->ChannelId);
		if (channel == nullptr)
		{
			client->Disconnect();
			return;
		}
		auto room = channel->FindRoom(mapId);
		if (room == nullptr)
		{
			client->Disconnect();
			return;
		}

		room->PushJob<GameRoomRef, PlayerRef, ClientRef>([room, player, client](GameRoomRef, PlayerRef, ClientRef) {
			room->Push(player);
			player->Room = room;

			// 입장하려는 유저에게 자신의 캐릭터 정보를 알림
			FlatBufferBuilder builder;
			auto myPlayerInfo = player->GenerateTotalInfo(builder);
			auto data = CreateSC_EnterMap(builder, player->GetMapId(), myPlayerInfo);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_EnterMap);
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
		if (client == nullptr)
		{
			session->Disconnect();
			return;
		}
		PlayerRef player = client->Player;
		if (player == nullptr)
		{
			client->Disconnect();
			return;
		}
		GameRoomRef room = player->Room;
		if (room == nullptr)
		{
			client->Disconnect();
			return;
		}

		// 룸 유저들에게 다른 유저의 입장을 알림
		{
			FlatBufferBuilder builder;
			auto playerInfo = player->GeneratePlayerInfo(builder);
			//auto monsterInfos = room->GetMonsterInfos(builder);

			auto data = CreateSC_PSpawn(builder, playerInfo);
			auto playerDataPkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_PSpawn);

			auto b = std::bind(static_cast<void(GameRoom::*)(SendBufferRef, PlayerRef)>(&GameRoom::Broadcast), room, std::placeholders::_1, std::placeholders::_2);
			room->PushJob<SendBufferRef, PlayerRef>(b, playerDataPkt, player);
		}

		{
			room->PushJob<GameRoomRef, ClientRef>([room, client](GameRoomRef, ClientRef) {
				FlatBufferBuilder builder;
				auto playerInfos = room->GetPlayerInfos(builder);
				auto data = CreateSC_CreatureInfos(builder, playerInfos);
				auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CreatureInfos);
				client->Send(pkt);
				}, room, client);
		}
	}
	catch (...)
	{

	}
}
