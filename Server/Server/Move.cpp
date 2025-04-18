#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::C_MoveStartHandler(PacketSession* session, ByteRef& buffer)
{
	auto client = Manager::Session.Find(session->GetSessionId());
	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}
	auto player = client->Player;
	if (player->IsInState(PlayerState::MOVE))
		return;

	auto pkt = GetRoot<C_MoveStart>(buffer->operator std::byte * ());
	try {
		auto room = player->Room;

		player->AddState(PlayerState::MOVE);
		room->PushJob<PlayerRef, MoveDirection, float, float>([player](PlayerRef, MoveDirection dir, float x, float y) {
			player->Pos->X = x;
			player->Pos->Y = y;

			FlatBufferBuilder builder;
			auto data = CreateSC_MoveStart(builder, player->Id, dir, player->Pos->X, player->Pos->Y);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_MoveStart);
			player->Room->Broadcast(packet, player);
			}, player, pkt->dir(), pkt->x(), pkt->y());
	}
	catch (...)
	{
	}
}

void PacketHandler::C_MoveEndHandler(PacketSession* session, ByteRef& buffer)
{
	auto client = Manager::Session.Find(session->GetSessionId());
	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}
	auto player = client->Player;
	if (player->IsInState(PlayerState::MOVE) == false)
		return;

	auto pkt = GetRoot<C_MoveEnd>(buffer->operator std::byte * ());
	try {
		auto room = player->Room;

		player->RemoveState(PlayerState::MOVE);
		room->PushJob<PlayerRef, float, float>([player](PlayerRef, float x, float y) {

			player->Pos->X = x;
			player->Pos->Y = y;
			FlatBufferBuilder builder;
			auto data = CreateSC_MoveEnd(builder, player->Id, player->Pos->X, player->Pos->Y);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_MoveEnd);
			player->Room->Broadcast(packet, player);
			}, player, pkt->x(), pkt->y());

	}
	catch (...)
	{

	}
}

void PacketHandler::C_JumpHandler(PacketSession* session, ByteRef& buffer)
{
	auto client = Manager::Session.Find(session->GetSessionId());
	if (client->State != ClientState::INGAME)
	{
		client->Disconnect();
		return;
	}
	auto player = client->Player;
	/*if (player->IsInState(PlayerState::JUMP))
		return;*/

	try {
		auto room = player->Room;

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
		if (client->State != ClientState::INGAME)
		{
			client->Disconnect();
			return;
		}
		auto player = client->Player;
		if (player->IsInState(PlayerState::PRONE_STAB))
			return;

		auto room = player->Room;
		player->AddState(PlayerState::PRONE_STAB);
		room->PushJob<PlayerRef>([player](PlayerRef) {

			FlatBufferBuilder builder;
			auto data = CreateSC_ProneStabStart(builder, player->Id);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_ProneStabStart);
			auto room = player->Room;
			room->Broadcast(packet, player);
			}, player);
	}
	catch (...)
	{

	}
}

void PacketHandler::C_ProneStabEndHandler(PacketSession* session, ByteRef& buffer)
{
	try {
		auto client = Manager::Session.Find(session->GetSessionId());
		if (client->State != ClientState::INGAME)
		{
			client->Disconnect();
			return;
		}
		auto player = client->Player;
		if (player->IsInState(PlayerState::PRONE_STAB) == false)
			return;
		auto room = player->Room;
		player->RemoveState(PlayerState::PRONE_STAB);
		room->PushJob<PlayerRef>([player](PlayerRef) {

			FlatBufferBuilder builder;
			auto data = CreateSC_ProneStabEnd(builder, player->Id);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_ProneStabEnd);
			player->Room->Broadcast(packet, player);
			}, player);
	}
	catch (...)
	{

	}
}
