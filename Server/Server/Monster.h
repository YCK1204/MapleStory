#pragma once
#include "Creature.h"

class Monster : public Creature {
public:
	virtual void TakeDamage(int32& damage);
	virtual const bool IsAlive() const;
	Monster();
	virtual ~Monster();

public:
	uint8 MonsterId;
	uint8 AttackableRange[2];
	int32 Hp;
	uint16 AttackPower;
	uint16 Armor;
	uint8 Speed;
};