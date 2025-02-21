#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::C_MoveStartHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<C_MoveStart>(buffer->operator std::byte * ());

	try {
		auto dir = pkt->dir();

		auto client = Manager::Session.Find(session->GetSessionId());

		if (client == nullptr)
		{
			session->Disconnect();
			return;
		}
		auto player = client->Player;
		if (player == nullptr)
		{
			client->Disconnect();
			return;
		}
		auto room = player->Room;
		if (room == nullptr)
		{
			client->Disconnect();
			return;
		}

		auto pos = player->Pos;
		pos->X = pkt->x();
		pos->Y = pkt->y();

		FlatBufferBuilder builder;
		auto data = CreateSC_MoveStart(builder, player->Id, dir, pos->X, pos->Y);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_MoveStart);
		room->Broadcast(packet);
	}
	catch (...)
	{
	}
}

void PacketHandler::C_MoveEndHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<C_MoveEnd>(buffer->operator std::byte * ()); 

	try {
		auto client = Manager::Session.Find(session->GetSessionId());

		if (client == nullptr)
		{
			session->Disconnect();
			return;
		}
		auto player = client->Player;
		if (player == nullptr)
		{
			client->Disconnect();
			return;
		}
		auto room = player->Room;
		if (room == nullptr)
		{
			client->Disconnect();
			return;
		}
		auto pos = player->Pos;
		pos->X = pkt->x();
		pos->Y = pkt->y();

		FlatBufferBuilder builder;
		auto data = CreateSC_MoveEnd(builder, player->Id, pos->X, pos->Y);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_MoveEnd);
		room->Broadcast(packet);
	}
	catch (...)
	{

	}
}
