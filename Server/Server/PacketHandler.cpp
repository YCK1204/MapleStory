#include "pch.h"
#include "PacketHandler.h"

void PacketHandler::C_SignUpHandler(PacketSession* session, ByteRef& buffer)
{
	const int32 lengthRangeStart = 4;
	const int32 lengthRangeEnd = 20;
	FlatBufferBuilder builder;

	try {
		auto pkt = GetRoot<C_SignUp>(reinterpret_cast<uint8*>(buffer->operator std::byte * ()));

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
		auto pkt = GetRoot<D_SignUp>(reinterpret_cast<uint8*>(buffer->operator std::byte * ()));
		auto error = pkt->ok();
		auto sessionId = pkt->session_id();

		auto data = CreateSC_SignUp(builder, error);
		auto bytes = Manager::Packet.CreatePacket(data, builder, PacketType_SC_SignUp);
		ClientSession* clientSession = Manager::Session.Find(sessionId);

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
	const int32 lengthRangeStart = 4;
	const int32 lengthRangeEnd = 20;
	FlatBufferBuilder builder;

	try {
		auto pkt = GetRoot<C_SignIn>(reinterpret_cast<uint8*>(buffer->operator std::byte * ()));

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
		auto pkt = GetRoot<D_SignIn>(reinterpret_cast<uint8*>(buffer->operator std::byte * ()));
		auto sessionId = pkt->session_id();
		ClientSession* clientSession = Manager::Session.Find(sessionId);

		if (clientSession != nullptr)
		{
			auto error = pkt->ok();
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