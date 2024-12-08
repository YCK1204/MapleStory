#include "pch.h"
#include "DbSession.h"
#include "Manager.h"

void DbSession::OnConnect()
{
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
