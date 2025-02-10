#include "pch.h"
#include "PacketHandler.h"

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
	auto data = CreateSC_Despawn(builder, client->Player->CharId);
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
		if (player == nullptr || player->CurMapId == -1)
		{
			client->Disconnect();
			return;
		}
		auto mapId = pkt->map_id();
		GameRoom* room = Manager::Room.Find(mapId);
		if (room == nullptr)
			return;
		FlatBufferBuilder builder;

	}
	catch (...)
	{

	}
}