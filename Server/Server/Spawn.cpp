#include "pch.h"
#include "PacketHandler.h"
#include "functional"

// [ ��Ŷ ũ�� ushort 2byte][ ��Ŷ ���� ushort  ] [uint64][uint64][ushort]

void PacketHandler::C_DespawnHandler(PacketSession* session, ByteRef& buffer)
{
	// �÷��̾� ���� ���� �ڵ� �߰� �ʿ�

	ClientSession* client = reinterpret_cast<ClientSession*>(session);

	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}
	GameRoomRef room = client->Player->Room;

	FlatBufferBuilder builder;

	auto data = CreateSC_Despawn(builder, client->Player->GetCharId()); // <- �� �ڵ� ���� ������ ���׳� �����ؾ��� ////////
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_Despawn);

	room->Remove(client->Player->Id);
	room->Broadcast(packet);
}

void PacketHandler::C_EnterMapHandler(PacketSession* session, ByteRef& buffer)
{
	ClientSession* client = reinterpret_cast<ClientSession*>(session);

	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}

	auto pkt = GetRoot<C_EnterMap>(buffer->operator std::byte * ());

	try {
		PlayerRef player = client->Player;
		auto mapId = pkt->map_id();

		// �볢�� �̵� ���� ��ȿ�� �˻� �ʿ�
		auto server = Manager::Server.Find(client->ServerId);
		auto channel = server->FindChannel(client->ChannelId);
		auto room = channel->FindRoom(mapId);
		if (room == nullptr)
		{
			client->Disconnect();
			return;
		}

		// ���� �뿡�� �ڽ��� ����
		{
			auto b = std::bind(static_cast<void(GameRoom::*)(PlayerRef)>(&GameRoom::Remove), player->Room, std::placeholders::_1);
			player->Room->PushJob<PlayerRef&>(b, player);
		}
		{
			auto b = std::bind(static_cast<void(GameRoom::*)(PlayerRef)>(&GameRoom::Push), room, std::placeholders::_1);
			room->PushJob<PlayerRef>(b, player);
		}
		player->Room = room;

		FlatBufferBuilder builder;
		auto myPlayerInfo = player->GenerateTotalInfo(builder);
		auto data = CreateSC_EnterMap(builder, player->GetMapId(), myPlayerInfo);
		auto pacekt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_EnterMap);
		client->Send(pacekt);
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

		player->Room = room;
		room->PushJob<GameRoomRef, PlayerRef, ClientRef>([room, player, client](GameRoomRef, PlayerRef, ClientRef) {
			room->Push(player);

			// �����Ϸ��� �������� �ڽ��� ĳ���� ������ �˸�
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
		if (client->State != ClientState::INGAME)
		{
			client->Disconnect();
			return;
		}

		PlayerRef player = client->Player;
		GameRoomRef room = player->Room;

		room->PushJob<GameRoomRef, ClientRef>([room, client](GameRoomRef, ClientRef) {
			FlatBufferBuilder builder;
			auto playerInfos = room->GetPlayerInfos(builder);
			auto data = CreateSC_CreatureInfos(builder, playerInfos);
			auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CreatureInfos);
			client->Send(pkt);
			}, room, client);
	}
	catch (...)
	{

	}
}
