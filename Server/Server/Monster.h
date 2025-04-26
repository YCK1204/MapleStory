#pragma once
#include "Creature.h"

class Monster : public Creature {
public:
	virtual void TakeDamage(const int32& damage);
	virtual const bool IsAlive() const;
	Monster();
	virtual ~Monster();
public:
	void Update();
	void SetState(MonsterState state);
	const MonsterState& GetState() const;
private:
	void UpdateStand();
	void UpdateMove();
	void UpdateTrace();
	void UpdateDie();
	void UpdateAttack();
	void UpdateHit();
private: 
// Patrol
	const static uint16 RanPatrolTickTime = 10000;
	uint64 LastPatrolUpdate = GetTickCount64();
	uint64 RanTickTime = GetTickCount64();
// Hit
	const static uint16 HitTickTime = 1000;
	uint64 LastHitUpdate = GetTickCount64();
// Trace
	const static uint8 TraceRange = 8;

private:
	MonsterState State = MonsterState::MonsterState_Stand;
public:
	float DestPosX;
	shared_ptr<struct SpawnInfo> SpawnInfo;
	shared_ptr<class Map> Map;
	uint8 MonsterId;
	uint8 AttackableRange[2];
	int32 Hp;
	uint16 AttackPower;
	uint16 Armor;
	uint8 Speed;
	weak_ptr<Player> Target;
public:
	Offset<MonsterInfo> GenerateMonsterInfo(FlatBufferBuilder& builder);
	Offset<MonsterInfoDetail> GenerateMonsterInfoDetail(FlatBufferBuilder& builder);
};