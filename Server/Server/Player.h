#pragma once

#include "Creature.h"
#include "GameRoom.h"

class Player : public Creature
{
public:
	int32 Mp;
public:
	Player();
	virtual ~Player();

public:
	virtual void TakeDamage(int32& damage);
};