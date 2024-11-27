#include "pch.h"
#include "ServerSession.h"
#include "Manager.h"

void ServerSession::OnConnect()
{
	cout << "serversession onconnect\n";
	Init();
	RegisterRecv();
}

void ServerSession::OnDisconnect()
{
}

void ServerSession::OnSend()
{
	cout << "OnSend\n";
}

void ServerSession::OnRecvPacket(int32 size, byte* data)
{
	Manager::Packet.OnRecvPacket(this, data);
}
