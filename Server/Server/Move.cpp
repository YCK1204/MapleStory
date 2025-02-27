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

		auto b = std::bind(static_cast<void(GameRoom::*)(SendBufferRef, PlayerRef)>(&GameRoom::Broadcast), room, std::placeholders::_1, std::placeholders::_2);
		room->PushJob<SendBufferRef, PlayerRef>(b, packet, player);
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
		auto b = std::bind(static_cast<void(GameRoom::*)(SendBufferRef, PlayerRef)>(&GameRoom::Broadcast), room, std::placeholders::_1, std::placeholders::_2);
		room->PushJob<SendBufferRef, PlayerRef>(b, packet, player);
	}
	catch (...)
	{

	}
}

void PacketHandler::C_JumpHandler(PacketSession* session, ByteRef& buffer)
{
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

		FlatBufferBuilder builder;
		auto data = CreateSC_Jump(builder, player->Id);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_Jump);
		auto b = std::bind(static_cast<void(GameRoom::*)(SendBufferRef, PlayerRef)>(&GameRoom::Broadcast), room, std::placeholders::_1, std::placeholders::_2);
		room->PushJob<SendBufferRef, PlayerRef>(b, packet, player);
	}
	catch (...)
	{

	}
}

void PacketHandler::C_ProneStabStartHandler(PacketSession* session, ByteRef& buffer)
{
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

		FlatBufferBuilder builder;
		auto data = CreateSC_ProneStabStart(builder, player->Id);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_ProneStabStart);
		auto b = std::bind(static_cast<void(GameRoom::*)(SendBufferRef, PlayerRef)>(&GameRoom::Broadcast), room, std::placeholders::_1, std::placeholders::_2);
		room->PushJob<SendBufferRef, PlayerRef>(b, packet, player);
	}
	catch (...)
	{

	}
}

void PacketHandler::C_ProneStabEndHandler(PacketSession* session, ByteRef& buffer)
{
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

		FlatBufferBuilder builder;
		auto data = CreateSC_ProneStabEnd(builder, player->Id);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_ProneStabEnd);
		auto b = std::bind(static_cast<void(GameRoom::*)(SendBufferRef, PlayerRef)>(&GameRoom::Broadcast), room, std::placeholders::_1, std::placeholders::_2);
		room->PushJob<SendBufferRef, PlayerRef>(b, packet, player);
	}
	catch (...)
	{

	}
}