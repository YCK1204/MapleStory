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
protected:
	PosInfo _posInfo;
	int32 _hp;
	CreatureState _state;
	GameRoomRef _room;

protected:
	const bool IsAlive() const;
	const int32& GetHp() const;
	const PosInfo& GetPosInfo() const;
	void SetHp(int32& hp);
	void SetPosInfo(int16 x, int16 y);
	void SetPosInfo(PosInfo posInfo);
public:
	Creature();
	virtual ~Creature();
	void EnterRoom(uint32& roomId);
	void LeaveRoom(uint32& roomId);
	void SetState(CreatureState state);
	const CreatureState& GetState() const;

public:
	virtual void TakeDamage(int32& damage) = 0;
};