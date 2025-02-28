#pragma once
#include "Session.h"
#include "Player.h"

enum class ClientState : uint8 {
	LOGIN,
	WORLD_SELECT,
	CHARACTER_SELECT,
	INGAME,
};

class ClientSession : public PacketSession
{
public:
	ClientState State = ClientState::LOGIN;
	PlayerRef Player = shared_ptr<class Player>(new class Player());
	uint8 ServerId;
	uint8 ChannelId;
private:
	void OnConnect() override;
	void OnDisconnect() override;
	void OnSend() override;
	void OnRecvPacket(int32 size, byte* data) override;
};