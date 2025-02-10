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
