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

void PacketHandler::C_CharacterSelectHandler(PacketSession* session, ByteRef& buffer)
{
	try {
		auto pkt = GetRoot<C_CharacterSelect>(buffer->operator std::byte * ());
		ClientRef client = Manager::Session.Find(session->GetSessionId());
		if (client == nullptr)
		{
			session->Disconnect();
			return;
		}
		auto charId = pkt->char_id();
		auto sessionId = session->GetSessionId();
		auto dbId = session->GetDbId();

		FlatBufferBuilder builder;
		auto data = CreateSD_CharacterSelect(builder, sessionId, dbId, charId);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SD_CharacterSelect);
		Manager::Session.dbSession->Send(packet);
	}
	catch (exception& e)
	{
		FlatBufferBuilder builder;
		auto data = CreateSC_CharacterSelect(builder, CharacterSelectError::CharacterSelectError_UNKNOWN);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CharacterSelect);
		session->Send(packet);
	}
}

void PacketHandler::D_CharacterSelectHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<D_CharacterSelect>(buffer->operator std::byte * ());

	if (pkt->ok() != CharacterSelectError_SUCCESS)
		return;
	ClientRef client = Manager::Session.Find(pkt->session_id());
	if (client == nullptr)
		return;
	client->Player = shared_ptr<Player>(new Player(client));
	FlatBufferBuilder builder;

	auto info = pkt->char_info();
	auto prevInfo = info->prev_info();
	auto ability = prevInfo->ability();
	auto lastPos = info->last_pos();

	client->Player->CurMapId = lastPos;

	auto newAbility = CreateCharacterAbility(builder, ability->STR(), ability->DEX(), ability->INT(), ability->LUK());
	auto newPrevInfo = CreateCharacterPreviewInfoDirect(builder, prevInfo->char_id(), prevInfo->char_type(), prevInfo->level(), prevInfo->name()->c_str());
	auto newTotalinfo = CreateCharacterTotalInfo(builder, newPrevInfo, lastPos, info->hp(), info->mp(), info->exp());

	auto data = CreateSC_CharacterSelect(builder, newTotalinfo);
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CharacterSelect);
	client->Send(packet);
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
