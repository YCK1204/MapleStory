#include "pch.h"
#include "Monster.h"
#include "RandomNumberGenerator.h"
#include "GameRoom.h"

void Monster::Update() {

	switch (State)
	{
	case MonsterState::MonsterState_Stand:
		UpdateStand();
		break;
	case MonsterState::MonsterState_Attack:
		UpdateAttack();
		break;
	case MonsterState::MonsterState_Hit:
		UpdateHit();
		break;
	case MonsterState::MonsterState_Die:
		UpdateDie();
		break;
	case MonsterState::MonsterState_Move:
		UpdateMove();
		break;
	case MonsterState::MonsterState_Trace:
		UpdateTrace();
		break;
	}
}

void Monster::UpdateStand() {
	if (LastPatrolUpdate + RanTickTime > GetTickCount64())
		return;

	auto minX = this->SpawnInfo->RangeX[0];
	auto maxX = this->SpawnInfo->RangeX[1];

	auto ran = RandomNumberGenerator::getRandomFloat(minX, maxX, 2);

	DestPosX = (float)ran;
	SetState(MonsterState::MonsterState_Move);
}

void Monster::UpdateMove() {
	auto nextPos = Pos->operator + (((Pos->X > DestPosX) ? Vector2::left : Vector2::right) * (float)Speed * .25f);

	if (Map->CanGo(nextPos, false) == false || ::abs(DestPosX - Pos->X) < .1f)
		SetState(MonsterState::MonsterState_Stand);
	else
		Pos->Copy(nextPos);
}

void Monster::UpdateTrace() {
	// 타겟이 없어진 경우, 서로 방이 다른 경우, 타겟이 추적 범위를 벗어난 경우 Idle 전환
	auto target = Target.lock();
	if (target == nullptr || target->Room == nullptr || target->Room->GetRoomId() != Room->GetRoomId() || (int32)(::abs(target->Pos->Y - Pos->Y) + ::abs(target->Pos->X - Pos->X) > TraceRange))
	{
		SetState(MonsterState::MonsterState_Stand);
		return;
	}

	DestPosX = target->Pos->X;
	auto nextPos = Pos->operator + (((Pos->X > DestPosX) ? Vector2::left : Vector2::right) * (float)Speed * .25f);

	if (Map->CanGo(nextPos, false) == true)
		Pos->Copy(nextPos);
}

void Monster::UpdateDie() {
}

void Monster::UpdateAttack() {
}

void Monster::UpdateHit() {
	if (LastHitUpdate + HitTickTime > GetTickCount64())
		return;
	SetState(MonsterState::MonsterState_Trace);
}
