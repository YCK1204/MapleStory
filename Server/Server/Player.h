#pragma once

#include "Creature.h"
#include "GameRoom.h"

enum class PlayerState {
	Lobby,
	InGame,
	CashShop,
};

class Player : public Creature
{
public:
	PlayerState State;
	int32 Mp;
public:
	Player();
	virtual ~Player();

public:
	virtual void TakeDamage(int32& damage);
};