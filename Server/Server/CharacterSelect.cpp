#include "pch.h"
#include "PacketHandler.h"

// [ 패킷 크기 ushort 2byte][ 패킷 종류 ushort  ] [uint64][uint64][ushort]

void PacketHandler::C_CharacterListHandler(PacketSession* session, ByteRef& buffer)
{
	ClientSession* client = reinterpret_cast<ClientSession*>(session);

	if (client->State != ClientState::CHARACTER_SELECT)
	{
		client->Disconnect();
		return;
	}

	FlatBufferBuilder builder;
	auto data = CreateSD_CharacterList(
		builder,
		session->GetSessionId(),
		session->GetDbId(),
		client->ServerId);

	auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SD_CharacterList);
	Manager::Session.dbSession->Send(pkt);
}

void PacketHandler::D_CharacterListHandler(PacketSession* session, ByteRef& buffer)
{
	auto pkt = GetRoot<D_CharacterList>(buffer->operator std::byte * ());

	ClientRef client = Manager::Session.Find(pkt->session_id());
	if (client == nullptr)
		return;

	FlatBufferBuilder builder;
	auto ok = pkt->ok();
	auto list = pkt->list();
	vector<Offset<CharacterInfo>> infoList;
	for (auto it = list->begin(); it != list->end(); it++)
	{
		auto ability = CreateCharacterAbility(builder,
			it->ability()->STR(),
			it->ability()->DEX(),
			it->ability()->INT(),
			it->ability()->LUK());
		infoList.push_back(CreateCharacterInfo(
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

	ClientSession* client = reinterpret_cast<ClientSession*>(session);

	if (client->State != ClientState::CHARACTER_SELECT)
	{
		client->Disconnect();
		return;
	}

	FlatBufferBuilder builder;

	try {
		auto pkt = GetRoot<C_CharacterDelete>(buffer->operator std::byte * ());

		auto data = CreateSD_CharacterDelete(
			builder,
			client->GetDbId(),
			client->GetSessionId(),
			pkt->char_id());
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SD_CharacterDelete);
		Manager::Session.dbSession->Send(packet);
	}
	catch (...)
	{
		auto data = CreateSC_CharacterDelete(builder, CharacterDeleteError_UNKNOWN);
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CharacterDelete);
		session->Send(packet);
	}
}

void PacketHandler::D_CharacterDeleteHandler(PacketSession* session, ByteRef& buffer) {
	auto pkt = GetRoot<D_CharacterDelete>(buffer->operator std::byte * ());

	ClientRef client = Manager::Session.Find(pkt->session_id());
	if (client == nullptr)
		return;

	FlatBufferBuilder builder;
	auto data = CreateSC_CharacterDelete(builder, pkt->ok());
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CharacterDelete);
	client->Send(packet);
}

void PacketHandler::C_CharacterSelectHandler(PacketSession* session, ByteRef& buffer)
{
	ClientSession* client = reinterpret_cast<ClientSession*>(session);

	if (client->State != ClientState::CHARACTER_SELECT)
	{
		client->Disconnect();
		return;
	}

	try {
		auto pkt = GetRoot<C_CharacterSelect>(buffer->operator std::byte * ());
		
		FlatBufferBuilder builder;
		auto data = CreateSD_CharacterSelect(
			builder,
			session->GetSessionId(),
			session->GetDbId(),
			pkt->char_id());
		auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SD_CharacterSelect);
		Manager::Session.dbSession->Send(packet);
	}
	catch (exception& e)
	{
		// 사실 에러날게 없지만 UNKNOWN 핸들링 필요
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

	auto info = pkt->char_info();
	auto prevInfo = info->prev_info();
	auto ability = prevInfo->ability();
	auto lastPos = info->last_pos();
	if (lastPos == 0)
		lastPos = 1;

	auto& player = client->Player;
	{
		player->SetAbility(ability);
		player->SetLevel(prevInfo->level());
		player->SetName(prevInfo->name()->str());
		player->SetCharId(prevInfo->char_id());
		player->SetCharType(prevInfo->char_type());
		player->SetMapId(lastPos);
		client->State = ClientState::INGAME;
	}

	FlatBufferBuilder builder;

	auto newAbility = CreateCharacterAbility(builder, ability->STR(), ability->DEX(), ability->INT(), ability->LUK());
	auto newPrevInfo = CreateCharacterInfoDirect(builder, prevInfo->char_id(), prevInfo->char_type(), prevInfo->level(), prevInfo->name()->c_str());
	auto newTotalinfo = CreateCharacterInfoDetail(builder, newPrevInfo, lastPos, info->hp(), info->mp(), info->exp());

	auto data = CreateSC_CharacterSelect(builder, newTotalinfo);
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CharacterSelect);
	client->Send(packet);
}
