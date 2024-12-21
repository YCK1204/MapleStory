#pragma once

#include "GameObject.h"
#include "GameRoom.h"

typedef struct {
	int16 X;
	int16 Y;
} PosInfo;

enum class CreatureState :uint8 {
	DEAD,
	LOBBY,
	ALIVE,
};

class Creature : public GameObject
{
public:
	PosInfo Pos;
	int32 Hp;
	CreatureState State;
	GameRoomRef Room;
	uint16 ChannelId;
	uint16 ServerId;
protected:
	const bool IsAlive() const;
public:
	virtual ~Creature();
	void EnterRoom(uint32& roomId);
	void LeaveRoom(uint32& roomId);

public:
	virtual void TakeDamage(int32& damage) = 0;
};