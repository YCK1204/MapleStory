#pragma once

#include "Session.h"

class ServerSession : public PacketSession
{
	virtual void OnConnect() override;
	virtual void OnDisconnect() override;
	virtual void OnSend() override;
	virtual void OnRecvPacket(int32 size, byte* data) override;
};