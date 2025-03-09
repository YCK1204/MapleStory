#pragma once

#include "GameObject.h"

typedef struct {
	float X ;
	float Y;
} PosInfo;

class Creature : public GameObject
{
public:
	shared_ptr<PosInfo> Pos = make_shared<PosInfo>();
public:
	Creature();
	virtual ~Creature();

public:
	Offset<Position> GeneratePosition(FlatBufferBuilder& builder);
	virtual const bool IsAlive() const = 0;
	virtual void TakeDamage(int32& damage) = 0;
};