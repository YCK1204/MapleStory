#include "pch.h"
#include "Monster.h"
#include "RandomNumberGenerator.h"
#include "GameRoom.h"

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
	RanTickTime = RandomNumberGenerator::getRandomInt(0, 5000);
}

Monster::~Monster()
{
}

bool Monster::CanPatrol()
{
	return (State == MonsterState::MonsterState_Stand && LastPatrolUpdate + RanTickTime < GetTickCount64());
}

Offset<MonsterInfo> Monster::Patrol(FlatBufferBuilder& builder)
{
	auto minX = this->SpawnInfo->RangeX[0];
	auto maxX = this->SpawnInfo->RangeX[1];

	auto ran = RandomNumberGenerator::getRandomFloat(minX, maxX, 2);

	DestPosX = (float)ran;
	State = MonsterState::MonsterState_Move;

	return GenerateMonsterInfo(builder);
}

void Monster::EndPatrol()
{
	LastPatrolUpdate = ::GetTickCount64();
	State = MonsterState::MonsterState_Stand;
	RanTickTime = RandomNumberGenerator::getRandomInt(0, 15000);
}

Offset<MonsterInfo> Monster::GenerateMonsterInfo(FlatBufferBuilder& builder)
{
	auto position = GeneratePosition(builder);
	return CreateMonsterInfo(builder, MonsterId, Id, position, DestPosX);
}
