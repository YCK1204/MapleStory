#include "pch.h"
#include "DbSession.h"
#include "Manager.h"

void DbSession::OnConnect()
{
	flatbuffers::FlatBufferBuilder builder;
	auto id = builder.CreateString("test id");
	auto password = builder.CreateString("test password");

	auto login = CreateS_Login(builder, id, password);

	auto data = Manager::Packet.CreatePacket(login, builder, PacketType_S_Login);
	Send(data);
	cout << "DbSession OnConnect\n";
	Init();
	RegisterRecv();
}

void DbSession::OnDisconnect()
{
}

void DbSession::OnSend()
{
	cout << "DbSession OnSend\n";
}

void DbSession::OnRecvPacket(int32 size, byte* data)
{
	Manager::Packet.OnRecvPacket(this, data);
}
