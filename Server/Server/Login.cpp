#include "pch.h"
#include "PacketHandler.h"

// [ ��Ŷ ũ�� ushort 2byte][ ��Ŷ ���� ushort  ] [uint64][uint64][ushort]

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

		// ��ȿ�� �˻�
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
		// db ������ SignUp ��û
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
		// ��Ŷ ó�� �� ���� �߻� �� ��� UNknown ��ȯ
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
		// ��� ���ܰ� �߻��� ���� ������ �׳� try catch
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

		// ��ȿ�� �˻�
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
		// db ������ SignIn ��û
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
		// ��Ŷ ó�� �� ���� �߻� �� ��� UNknown ��ȯ
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
		// ��� ���ܰ� �߻��� ���� ������ �׳� try catch
		cerr << e.what() << endl;
	}
}
