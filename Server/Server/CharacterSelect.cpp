#include "pch.h"
#include "PacketHandler.h"

// [ 패킷 크기 ushort 2byte][ 패킷 종류 ushort  ] [uint64][uint64][ushort]

void PacketHandler::C_CharacterListHandler(PacketSession* session, ByteRef& buffer)
{
	auto sessionId = session->GetSessionId();
	ClientRef client = Manager::Session.Find(sessionId);
	if (client == nullptr)
	{
		session->Disconnect();
		return;
	}

	FlatBufferBuilder builder;
	auto dbId = session->GetDbId();
	auto data = CreateSD_CharacterList(builder, sessionId, dbId, client->ServerId);
	auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SD_CharacterList);
	Manager::Session.dbSession->Send(pkt);
}

void PacketHandler::D_CharacterListHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<D_CharacterList>(buffer->operator std::byte * ());
	FlatBufferBuilder builder;

	ClientRef client = Manager::Session.Find(pkt->session_id());
	if (client == nullptr)
		return;

	auto ok = pkt->ok();
	auto list = pkt->list();
	vector<Offset<CharacterPreviewInfo>> infoList;
	for (auto it = list->begin(); it != list->end(); it++)
	{
		auto ability = CreateCharacterAbility(builder,
			it->ability()->STR(),
			it->ability()->DEX(),
			it->ability()->INT(),
			it->ability()->LUK());
		infoList.push_back(CreateCharacterPreviewInfo(
			builder,
			it->char_id(),
			it->char_type(),
			it->level(),
			builder.CreateString(it->name()),
			ability
		));
	}
	auto vecList = builder.CreateVector(infoList);
	auto data = CreateSC_CharacterList(builder, ok, vecList);
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CharacterList);
	client->Send(packet);
}

void PacketHandler::C_CharacterDeleteHandler(PacketSession* session, ByteRef& buffer) {
	try {
		auto pkt = GetRoot<C_CharacterDelete>(buffer->operator std::byte * ());
		FlatBufferBuilder builder;

		auto sessionId = session->GetSessionId();
		ClientRef client = Manager::Session.Find(sessionId);
		if (client == nullptr)
			return;

		auto data = CreateSD_CharacterDelete(builder, client->GetDbId(), client->GetSessionId(), pkt->char_id());
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SD_CharacterDelete);
		Manager::Session.dbSession->Send(packet);
	}
	catch (...)
	{
		FlatBufferBuilder builder;
		auto data = CreateSC_CharacterDelete(builder, CharacterDeleteError_UNKNOWN);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CharacterDelete);
		session->Send(packet);
	}
}

void PacketHandler::D_CharacterDeleteHandler(PacketSession* session, ByteRef& buffer) {
	auto pkt = GetRoot<D_CharacterDelete>(buffer->operator std::byte * ());
	FlatBufferBuilder builder;

	ClientRef client = Manager::Session.Find(pkt->session_id());
	if (client == nullptr)
		return;
	auto data = CreateSC_CharacterDelete(builder, pkt->ok());
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CharacterDelete);
	client->Send(packet);
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
