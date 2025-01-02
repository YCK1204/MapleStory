#include "pch.h"
#include "PacketHandler.h"

// [ 패킷 크기 ushort 2byte][ 패킷 종류 ushort  ] [uint64][uint64][ushort]

void PacketHandler::C_SignUpHandler(PacketSession* session, ByteRef& buffer)
{
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

		ClientSession* clientSession = Manager::Session.Find(sessionId);
		if (error == SignInError_SUCCESS)
		{
			clientSession->SetDbId(pkt->db_id());
			clientSession->Player = shared_ptr<Player>(new Player());
		}
		auto data = CreateSC_SignUp(builder, error);
		auto bytes = Manager::Packet.CreatePacket(data, builder, PacketType_SC_SignUp);

		if (clientSession != nullptr)
			clientSession->Send(bytes);
	}
	catch (exception& e)
	{
		// 사실 예외가 발생할 일이 없지만 그냥 try catch
		cerr << e.what() << endl;
	}
}

void PacketHandler::C_SignInHandler(PacketSession* session, ByteRef& buffer)
{
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
		ClientSession* clientSession = Manager::Session.Find(sessionId);

		if (clientSession != nullptr)
		{
			auto error = pkt->ok();

			if (error == SignInError_SUCCESS)
				clientSession->Player = shared_ptr<Player>(new Player());
			auto data = CreateSC_SignIn(builder, sessionId, error);
			auto bytes = Manager::Packet.CreatePacket(data, builder, PacketType_SC_SignIn);
			clientSession->SetDbId(pkt->db_id());
			clientSession->Send(bytes);
		}
	}
	catch (exception& e)
	{
		// 사실 예외가 발생할 일이 없지만 그냥 try catch
		cerr << e.what() << endl;
	}
}

void PacketHandler::C_SignOutHandler(PacketSession* session, ByteRef& buffer)
{
}

void PacketHandler::C_EnterChannelHandler(PacketSession* session, ByteRef& buffer)
{
	FlatBufferBuilder builder;
	ClientSession* clientSession = reinterpret_cast<ClientSession*>(session);

	EnterChannelError error = EnterChannelError::EnterChannelError_SUCCESS;
	try {
		Player* player = clientSession->Player.get();
		if (player == nullptr || player->State != PlayerState::Lobby)
			return;

		auto pkt = GetRoot<C_EnterChannel>(reinterpret_cast<uint8*>(buffer->operator std::byte * ()));
		uint8 channelId = pkt->channel_index();
		uint8 serverId = pkt->server_index();

		Server* server = Manager::Server.Find(serverId);
		if (server == nullptr)
			return;
		Channel* channel = server->FindChannel(channelId);
		if (channel == nullptr)
			return;

		if (channel->GetUserCount() >= server->GetMaxUserCount())
			error = EnterChannelError::EnterChannelError_FULL;
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
		error = EnterChannelError::EnterChannelError_UNKNOWN;
	}
	
	auto data = CreateSC_EnterChannel(builder, error);
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_EnterChannel);
	clientSession->Send(packet);
}

void PacketHandler::C_ChannelInfoHandler(PacketSession* session, ByteRef& buffer)
{
	ClientSession* clientSession = reinterpret_cast<ClientSession*>(session);
	FlatBufferBuilder builder;

	try {
		Player* player = clientSession->Player.get();
		if (player == nullptr || player->State != PlayerState::Lobby)
			return;

		auto pkt = GetRoot<C_ChannelInfo>(reinterpret_cast<uint8*>(buffer->operator std::byte * ()));
		uint8 serverId = pkt->server_id();

		Server* server = Manager::Server.Find(serverId);
		if (server == nullptr)
			clientSession->Disconnect();
		else
		{
			auto data = Manager::Server.GetChannelInfo(server, builder);
			auto packet = Manager::Packet.CreatePacket(data, builder, PacketType::PacketType_SC_ChannelInfo);
			clientSession->Send(packet);
		}
	}
	catch (exception& e)
	{
		cerr << e.what() << endl;
	}
}
void PacketHandler::C_CreateCharacterHandler(PacketSession* session, ByteRef& buffer)
{

}

void PacketHandler::D_CreateCharacterHandler(PacketSession* session, ByteRef& buffer)
{

}