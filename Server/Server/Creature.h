#pragma once

#include "GameObject.h"
#include "GameRoom.h"

typedef struct {
	int16 X;
	int16 Y;
} PosInfo;

class Creature : public GameObject
{
public:
	PosInfo Pos;
	int32 Hp;
protected:
	const bool IsAlive() const;
public:
	virtual ~Creature();
	

public:
	virtual void TakeDamage(int32& damage) = 0;
};