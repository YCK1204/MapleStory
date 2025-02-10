#include "pch.h"
#include "PacketHandler.h"

// [ 패킷 크기 ushort 2byte][ 패킷 종류 ushort  ] [uint64][uint64][ushort]

void PacketHandler::C_CheckNameHandler(PacketSession* session, ByteRef& buffer)
{
	auto sessionId = session->GetSessionId();
	ClientRef client = Manager::Session.Find(sessionId);

	if (client == nullptr)
	{
		session->Disconnect();
		return;
	}
	FlatBufferBuilder builder;

	try
	{
		auto checkName = GetRoot<C_CheckName>(buffer->operator std::byte * ());
		string name = checkName->name()->str();
		if (name.empty())
		{
			client->Disconnect();
			return;
		}
		auto data = CreateSD_CheckName(builder, sessionId, builder.CreateString(name));
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SD_CheckName);
		Manager::Session.dbSession->Send(pkt);
	}
	catch (...)
	{
		FlatBufferBuilder builder;
		auto data = CreateSC_CheckName(builder, CheckNameError_UNKNOWN);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CheckName);
		session->Send(pkt);
	}
}

void PacketHandler::D_CheckNameHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<D_CheckName>(buffer->operator std::byte * ());

	try {
		ClientRef client = Manager::Session.Find(pkt->session_id());
		if (client == nullptr)
			return;
		FlatBufferBuilder builder;

		auto data = CreateSC_CheckName(builder, pkt->ok());
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CheckName);
		client->Send(pkt);
	}
	catch (exception& e)
	{
	}
}

void PacketHandler::C_CreateCharacterHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<C_CreateCharacter>(buffer->operator std::byte * ());

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

		auto sessionId = session->GetSessionId();
		auto dbId = session->GetDbId();
		auto charType = pkt->char_type();
		auto name = pkt->name()->str();
		auto ability = pkt->ability();


		FlatBufferBuilder builder;

		auto offsetAbility = CreateCharacterAbility(
			builder,
			ability->STR(),
			ability->DEX(),
			ability->INT(),
			ability->LUK());

		auto data = CreateSD_CreateCharacter(
			builder,
			builder.CreateString(name),
			charType,
			sessionId,
			dbId,
			offsetAbility,
			client->ServerId);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SD_CreateCharacter);
		Manager::Session.dbSession->Send(pkt);
	}
	catch (exception& e)
	{
		FlatBufferBuilder builder;
		auto data = CreateSC_CreateCharacter(builder, CreateCharacterError_UNKNOWN);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CreateCharacter);
		session->Send(pkt);
	}
}

void PacketHandler::D_CreateCharacterHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<D_CreateCharacter>(buffer->operator std::byte * ());

	try {
		auto sessionId = pkt->session_id();

		ClientRef client = Manager::Session.Find(sessionId);
		if (client == nullptr)
			return;
		FlatBufferBuilder builder;
		auto data = CreateSC_CreateCharacter(builder, pkt->ok());
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CreateCharacter);
		client->Send(packet);
	}
	catch (exception& e)
	{

	}
}
