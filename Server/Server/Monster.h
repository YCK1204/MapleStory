#pragma once
#include "Creature.h"

class Monster : public Creature {
public:
	virtual void TakeDamage(int32& damage);
	virtual const bool IsAlive() const;
	Monster();
	virtual ~Monster();

private:
	const static uint64 RanPatrolTickTime = 10000;
	uint64 LastPatrolUpdate = GetTickCount64();
	uint64 RanTickTime;
public:
	bool CanPatrol();
	Offset<MonsterInfo> Patrol(FlatBufferBuilder& builder);
	void EndPatrol();
	Offset<MonsterInfo> GenerateMonsterInfo(FlatBufferBuilder& builder);
public:
	float DestPosX;
	MonsterState State = MonsterState::MonsterState_Stand;
	shared_ptr<struct SpawnInfo> SpawnInfo;
public:
	uint8 MonsterId;
	uint8 AttackableRange[2];
	int32 Hp;
	uint16 AttackPower;
	uint16 Armor;
	uint8 Speed;
};