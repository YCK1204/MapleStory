#pragma once

#include "Creature.h"
#include "GameRoom.h"

class Player : public Creature
{
private:
	int32 _mp;
public:
	Player();
	virtual ~Player();
	void SetMp(int32 mp);
	const int32 GetMp() const;
public:
	virtual void TakeDamage(int32& damage);
};