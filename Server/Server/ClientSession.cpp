#include "pch.h"
#include "ClientSession.h"
#include "Manager.h"

void ClientSession::OnConnect()
{
	Init();
	cout << "connected!" << endl;
	RegisterRecv();
	Manager::Session.Push(this);
}

void ClientSession::OnDisconnect()
{
	if (Player != nullptr && Player->Room != nullptr)
		Player->Room->Remove(Player);
	Player = nullptr;
	GPoolManager->Push<ClientSession>(this);
	Manager::Session.Remove(this->_sessionId);
	cout << "disconnected!\n";
}

void ClientSession::OnSend()
{
}

void ClientSession::OnRecvPacket(int32 size, byte* data)
{
	Manager::Packet.OnRecvPacket(this, data);
}
