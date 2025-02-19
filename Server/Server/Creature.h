#pragma once

#include "GameObject.h"
#include "GameRoom.h"

typedef struct {
	float X ;
	float Y;
} PosInfo;

class Creature : public GameObject
{
public:
	shared_ptr<PosInfo> Pos = make_shared<PosInfo>();
protected:
	virtual const bool IsAlive() const = 0;
public:
	Creature();
	virtual ~Creature();
	

public:
	virtual void TakeDamage(int32& damage) = 0;
};