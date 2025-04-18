#include "pch.h"
#include "GameRoom.h"
#include "RandomNumberGenerator.h"

void GameRoom::GenMonster()
{
	FlatBufferBuilder builder;
	vector<Offset<MonsterInfo>> monsterInfos;

	if (_spawnInfo.size() == 0)
		return;

	for (auto& spawnInfo : _spawnInfo)
	{
		auto& key = spawnInfo.first;
		auto& value = spawnInfo.second;
		if (value.size() >= key->spawnCount)
			continue;
		auto typeRange = key->monsterType.size() - 1;
		for (auto i = value.size(); i < key->spawnCount; i++)
		{
			auto ranInt = RandomNumberGenerator::getRandomInt(0, typeRange);
			auto typeId = key->monsterType[ranInt];
			auto clone = Manager::Data.MonsterClone(typeId);
			auto go = static_pointer_cast<GameObject>(clone);
			Push(go);

			clone->Pos->X = RandomNumberGenerator::getRandomInt(key->RangeX[0], key->RangeX[1]);
			clone->Pos->Y = key->Y;
			clone->DestPosX = clone->Pos->X;
			clone->SpawnInfo = spawnInfo.first;
			auto pos = clone->GeneratePosition(builder);
			auto monsterInfo = CreateMonsterInfo(builder, clone->MonsterId, clone->Id, pos);
			monsterInfos.push_back(monsterInfo);
			value.insert(clone);
		}
	}
	if (monsterInfos.size() == 0)
		return;
	auto infos = builder.CreateVector(monsterInfos);
	auto data = CreateSC_MSpawn(builder, infos);
	auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_MSpawn);
	Broadcast(pkt);
}
// todo
// Character 관련 구조체 Set Get 유틸 전역 함수 추가 필요

const bool GameRoom::CanPort(uint8 id) const
{
	auto portal = find_if(portals.begin(), portals.end(), [id](uint8 portalId) { return portalId == id; });
	return portal != portals.end();
}

void GameRoom::Update() {
	WRITE_LOCK;
	while (_jobQueue.size() > 0)
	{
		auto job = _jobQueue.top();
		if (job->CanExecute() == false)
			break;
		job->Execute();
		_jobQueue.pop();
	}
	auto tick = GetTickCount64();
	if (LastSpawnUpdate + SpawnUpdateTickTime < tick)
	{
		LastSpawnUpdate = tick;
		PushJob([this]() { GenMonster(); });
	}
	if (LastPatrolUpdate + PatrolUpdateTickTime < tick)
	{
		LastPatrolUpdate = tick;
		PushJob([this]() { UpdatePatrol(); });
	}
	if (LastMonsterSync + MonsterSyncTickTime < tick)
	{
		LastMonsterSync = tick;
		PushJob([this]() { UpdateMonster(); });
	}
}

void GameRoom::UpdatePatrol()
{
	vector<Offset<MonsterInfo>> vec;
	FlatBufferBuilder builder;

	for (auto object : _objects)
	{
		auto monster = static_pointer_cast<Monster>(object.second);
		if (monster->CanPatrol() == false)
			continue;
		auto moveData = monster->Patrol(builder);
		vec.push_back(moveData);
	}

	if (vec.size() > 0)
	{
		auto moveVector = builder.CreateVector(vec);
		auto data = CreateSC_MonsterMove(builder, moveVector);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_MonsterMove);
		Broadcast(pkt);
	}
}

float Check(float a, float b)
{
	if (b < 0 && a < 0)
		a = a + b;
	a = ::abs(a);
	b = ::abs(b);
	return ::abs(a - b);
}

void GameRoom::UpdateMonster()
{
	for (auto object : _objects)
	{
		auto monster = static_pointer_cast<Monster>(object.second);
		if (monster->State != MonsterState::MonsterState_Move)
			continue;
		auto pos = monster->Pos;
		auto nextPos = pos->operator + (((pos->X > monster->DestPosX) ? Vector2::left : Vector2::right) * (float)monster->Speed * ((float)MonsterSyncTickTime / 1000.0f));
		if (_map->CanGo(nextPos, false) == false || monster->DestPosX == pos->X)
			monster->EndPatrol();
		else
			pos->Copy(nextPos);
	}
}