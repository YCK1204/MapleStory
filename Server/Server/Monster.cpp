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

Offset<MonsterInfo> Monster::GenerateMonsterInfo(FlatBufferBuilder& builder)
{
	return CreateMonsterInfo(builder, Id, DestPosX, State);
}

Offset<MonsterInfoDetail> Monster::GenerateMonsterInfoDetail(FlatBufferBuilder& builder)
{
	auto info = GenerateMonsterInfo(builder);
	auto position = GeneratePosition(builder);

	return CreateMonsterInfoDetail(builder, MonsterId, position, info);
}


void Monster::SetState(MonsterState state)
{
	if (State == state)
		return;

	switch (State)
	{
	case MonsterState_Move:
		break;
	case MonsterState_Stand:
		break;
	case MonsterState_Hit:
		break;
	case MonsterState_Attack:
		break;
	case MonsterState_Die:
		break;
	case MonsterState_Trace:
		Target.reset();
		break;
	}

	switch (state)
	{
	case MonsterState::MonsterState_Stand:
		LastPatrolUpdate = ::GetTickCount64();
		State = MonsterState::MonsterState_Stand;
		RanTickTime = RandomNumberGenerator::getRandomInt(0, 15000);
		break;
	case MonsterState::MonsterState_Attack:
		break;
	case MonsterState::MonsterState_Hit:
		break;
	case MonsterState::MonsterState_Die:
		break;
	case MonsterState::MonsterState_Move:
		break;
	case MonsterState::MonsterState_Trace:
		break;
	}
	State = state;
}


const MonsterState& Monster::GetState() const
{
	return State;
}