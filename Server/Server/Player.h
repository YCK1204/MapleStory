#pragma once

#include "Creature.h"
#include "GameRoom.h"

class Player : public Creature
{
public:
	weak_ptr<ClientSession> Session;
	GameRoomRef Room = nullptr;

public:
	uint64 CharId;
	uint8 CharType;
	uint8 CurMapId = -1;
public:
	Player(ClientRef session);
	virtual ~Player();
	virtual void TakeDamage(int32& damage);

public:
	void EnterRoom(uint8& roomId);
	void LeaveRoom(uint8& roomId);
};