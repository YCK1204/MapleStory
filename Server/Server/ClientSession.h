#pragma once
#include "Session.h"

class ClientSession : public PacketSession
{
private:
	void OnConnect() override;
	void OnDisconnect() override;
	void OnSend() override;
	void OnRecvPacket(int32 size, byte* data) override;
};