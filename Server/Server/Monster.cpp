#include "pch.h"
#include "Monster.h"

void Monster::TakeDamage(int32& damage)
{
	Hp -= damage;
}

const bool Monster::IsAlive() const
{
	return Hp > 0;
}

Monster::Monster()
{
	Type = ObjectType::MONSTER;
}

Monster::~Monster()
{
}
