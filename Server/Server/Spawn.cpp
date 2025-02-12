#include "pch.h"
#include "PacketHandler.h"

// [ ��Ŷ ũ�� ushort 2byte][ ��Ŷ ���� ushort  ] [uint64][uint64][ushort]

void PacketHandler::C_DespawnHandler(PacketSession* session, ByteRef& buffer)
{
	// �÷��̾� ���� ���� �ڵ� �߰� �ʿ�

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
	auto data = CreateSC_Despawn(builder, client->Player->GetCharId());
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
		if (player == nullptr || player->CurMapId == -1 || player->CurMapId != mapId)
		{
			client->Disconnect();
			return;
		}
		GameRoom* room = Manager::Room.Find(mapId);
		if (room == nullptr)
			return;
		FlatBufferBuilder builder;

		// �� �����鿡�� �ٸ� ������ ������ �˸�
		{
			auto previewInfo = player->GenerateCharPreviewInfo(builder);
			auto data = CreateSC_Spawn(builder, previewInfo);
			auto playerDataPkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_Spawn);
			room->Broadcast(playerDataPkt);
		}

		room->Push(player);
		// �����Ϸ��� �������� �뿡�ִ� �������� ������ �˸�
		{
			auto playerInfos = room->GetPlayerInfos(builder);
			auto data = CreateSC_EnterMap(builder, playerInfos);
			auto playerInfosPkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_EnterMap);
			client->Send(playerInfosPkt);
		}
	}
	catch (...)
	{

	}
}