#pragma once

#include "GameObject.h"

class Creature : public GameObject
{
public:
	GameRoomRef Room = nullptr;
public:
	Creature();
	virtual ~Creature();

public:
	Offset<Position> GeneratePosition(FlatBufferBuilder& builder);
	virtual const bool IsAlive() const = 0;
	virtual void TakeDamage(const int32& damage) = 0;
};