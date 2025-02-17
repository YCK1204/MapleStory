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
protected:
	virtual const bool IsAlive() const = 0;
public:
	virtual ~Creature();
	

public:
	virtual void TakeDamage(int32& damage) = 0;
};