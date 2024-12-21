#pragma once
#include "Session.h"
#include "Player.h"

class ClientSession : public PacketSession
{
public:
	PlayerRef Player = nullptr;
private:
	void OnConnect() override;
	void OnDisconnect() override;
	void OnSend() override;
	void OnRecvPacket(int32 size, byte* data) override;
};