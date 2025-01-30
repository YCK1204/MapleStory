#include "pch.h"
#include "PacketHandler.h"

// [ 패킷 크기 ushort 2byte][ 패킷 종류 ushort  ] [uint64][uint64][ushort]

#pragma region PrevInGame
#pragma region Login
void PacketHandler::C_SignUpHandler(PacketSession* session, ByteRef& buffer)
{
	ClientRef client = Manager::Session.Find(session->GetSessionId());
	if (client == nullptr)
	{
		session->Disconnect();
		return;
	}

	FlatBufferBuilder builder;
	const int32 lengthRangeStart = 4;
	const int32 lengthRangeEnd = 20;

	try {
		auto pkt = GetRoot<C_SignUp>(reinterpret_cast<uint8*>(buffer->operator byte * ()));

		string id = pkt->user_id()->str();
		string password = pkt->password()->str();

		// 유효성 검사
		{
			if (!(lengthRangeStart <= id.length() && id.length() <= lengthRangeEnd) ||
				!(lengthRangeStart <= password.length() && password.length() <= lengthRangeEnd))
				return;

			for (char& c : id)
			{
				if (isalnum(c) == false)
					return;
			}
			for (char& c : password)
			{
				if (isalnum(c) == false)
					return;
			}
		}
		// db 서버에 SignUp 요청
		{
			auto fId = builder.CreateString(id);
			auto fPassword = builder.CreateString(password);
			auto sessionId = session->GetSessionId();

			auto data = CreateSD_SignUp(builder, sessionId, fId, fPassword);
			auto bytes = Manager::Packet.CreatePacket(data, builder, PacketType_SD_SignUp);
			Manager::Session.dbSession->Send(bytes);
		}
	}
	catch (exception& e)
	{
		// 패킷 처리 중 예외 발생 시 즉시 UNknown 반환
		cerr << e.what() << endl;
		auto data = CreateSC_SignUp(builder, SignUpError_UNKNOWN);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_SignUp);
		Manager::Session.dbSession->Send(pkt);
	}
}

void PacketHandler::D_SignUpHandler(PacketSession* session, ByteRef& buffer)
{
	FlatBufferBuilder builder;

	try {
		auto pkt = GetRoot<D_SignUp>(reinterpret_cast<uint8*>(buffer->operator byte * ()));
		auto error = pkt->ok();
		auto sessionId = pkt->session_id();

		ClientRef client = Manager::Session.Find(sessionId);
		if (client == nullptr)
			return;
		if (error == SignInError_SUCCESS)
			client->SetDbId(pkt->db_id());

		auto data = CreateSC_SignUp(builder, error);
		auto bytes = Manager::Packet.CreatePacket(data, builder, PacketType_SC_SignUp);
		client->Send(bytes);
	}
	catch (exception& e)
	{
		// 사실 예외가 발생할 일이 없지만 그냥 try catch
		cerr << e.what() << endl;
	}
}

void PacketHandler::C_SignInHandler(PacketSession* session, ByteRef& buffer)
{
	ClientRef client = Manager::Session.Find(session->GetSessionId());
	if (client == nullptr)
	{
		session->Disconnect();
		return;
	}
	FlatBufferBuilder builder;
	const int32 lengthRangeStart = 4;
	const int32 lengthRangeEnd = 20;

	try {
		auto pkt = GetRoot<C_SignIn>(reinterpret_cast<uint8*>(buffer->operator byte * ()));

		string id = pkt->user_id()->str();
		string password = pkt->password()->str();

		// 유효성 검사
		{
			if (!(lengthRangeStart <= id.length() && id.length() <= lengthRangeEnd) ||
				!(lengthRangeStart <= password.length() && password.length() <= lengthRangeEnd))
				return;

			for (char& c : id)
			{
				if (isalnum(c) == false)
					return;
			}
			for (char& c : password)
			{
				if (isalnum(c) == false)
					return;
			}
		}
		// db 서버에 SignIn 요청
		{
			auto fId = builder.CreateString(id);
			auto fPassword = builder.CreateString(password);
			auto sessionId = session->GetSessionId();

			auto data = CreateSD_SignIn(builder, sessionId, fId, fPassword);
			auto bytes = Manager::Packet.CreatePacket(data, builder, PacketType_SD_SignIn);
			Manager::Session.dbSession->Send(bytes);
		}
	}
	catch (exception& e)
	{
		// 패킷 처리 중 예외 발생 시 즉시 UNknown 반환
		cerr << e.what() << endl;
		auto data = CreateSC_SignIn(builder, SignUpError_UNKNOWN);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SD_SignIn);
		Manager::Session.dbSession->Send(pkt);
	}
}

void PacketHandler::D_SignInHandler(PacketSession* session, ByteRef& buffer)
{
	FlatBufferBuilder builder;

	try {
		auto pkt = GetRoot<D_SignIn>(reinterpret_cast<uint8*>(buffer->operator byte * ()));
		auto sessionId = pkt->session_id();
		ClientRef client = Manager::Session.Find(sessionId);

		if (client != nullptr)
		{
			auto error = pkt->ok();
			auto data = CreateSC_SignIn(builder, sessionId, error);
			auto bytes = Manager::Packet.CreatePacket(data, builder, PacketType_SC_SignIn);

			client->SetDbId(pkt->db_id());
			client->Send(bytes);
		}
	}
	catch (exception& e)
	{
		// 사실 예외가 발생할 일이 없지만 그냥 try catch
		cerr << e.what() << endl;
	}
}
#pragma endregion
#pragma region WorldSelect
void PacketHandler::C_EnterChannelHandler(PacketSession* session, ByteRef& buffer)
{
	FlatBufferBuilder builder;
	ClientSession* client = reinterpret_cast<ClientSession*>(session);

	EnterChannelError error = EnterChannelError::EnterChannelError_SUCCESS;
	try {
		auto pkt = GetRoot<C_EnterChannel>(buffer->operator std::byte * ());
		uint8 channelId = pkt->channel_index();
		uint8 serverId = pkt->server_index();

		Server* server = Manager::Server.Find(serverId);
		if (server == nullptr)
		{
			session->Disconnect();
			return;
		}
		Channel* channel = server->FindChannel(channelId);
		if (channel == nullptr)
		{
			session->Disconnect();
			return;
		}

		if (channel->GetUserCount() >= server->GetMaxUserCount())
			error = EnterChannelError::EnterChannelError_FULL;
		if (error == EnterChannelError_SUCCESS)
		{
			client->ServerId = serverId;
			client->ChannelId = channelId;
		}
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		error = EnterChannelError::EnterChannelError_UNKNOWN;
	}

	auto data = CreateSC_EnterChannel(builder, error);
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_EnterChannel);
	client->Send(packet);
}

void PacketHandler::C_ChannelInfoHandler(PacketSession* session, ByteRef& buffer)
{
	ClientRef client = Manager::Session.Find(session->GetSessionId());
	if (client == nullptr)
	{
		session->Disconnect();
		return;
	}
	FlatBufferBuilder builder;

	try {
		auto pkt = GetRoot<C_ChannelInfo>(buffer->operator std::byte * ());
		uint8 serverId = pkt->server_id();

		Server* server = Manager::Server.Find(serverId);
		if (server == nullptr)
			client->Disconnect();
		else
		{
			auto data = Manager::Server.GetChannelInfo(server, builder);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType::PacketType_SC_ChannelInfo);
			client->Send(packet);
		}
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}
}
#pragma endregion
#pragma region CharacterSelect
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

#pragma endregion
#pragma region CreateCharacter
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
	string lastPosStr = 
		(lastPos->str().empty() == true) ? 
		"{"
		"\"map\":1"
		"}" 
		:
		lastPos->str();

	auto j = json::parse(lastPosStr);
	client->Player->CurMapId = j["map"];

	auto newAbility = CreateCharacterAbility(builder, ability->STR(), ability->DEX(), ability->INT(), ability->LUK());
	auto newPrevInfo = CreateCharacterPreviewInfoDirect(builder, prevInfo->char_id(), prevInfo->char_type(), prevInfo->level(), prevInfo->name()->c_str());
	auto newTotalinfo = CreateCharacterTotalInfoDirect(builder, newPrevInfo, lastPosStr.c_str(), info->hp(), info->mp(), info->exp());

	auto data = CreateSC_CharacterSelect(builder, newTotalinfo);
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_CharacterSelect);
	client->Send(packet);
}

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
#pragma endregion
#pragma endregion