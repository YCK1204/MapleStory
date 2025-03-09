#include "pch.h"
#include "Creature.h"

Creature::Creature()
{
	Pos->X = 0;
	Pos->Y = 0;
}

Creature::~Creature()
{
}


Offset<Position> Creature::GeneratePosition(FlatBufferBuilder& builder)
{
	return CreatePosition(builder, Pos->X, Pos->Y);
}