#include "pch.h"
#include "MonsterManager.h"
#include "Monster.h"

MonsterManager* MonsterManager::_instance = nullptr;

MonsterManager& MonsterManager::Instance()
{
	if (_instance == nullptr)
		_instance = new MonsterManager();

	return *_instance;
}

void MonsterManager::Init(json& monsterJson)
{
	json monsters = monsterJson["monsters"];
	GPoolManager->CreatePool<Monster>(10, 1000);
	for (auto monster : monsters)
	{
		auto clone = make_shared<Monster>();

		clone->MonsterId = monster["id"];
		clone->AttackableRange = monster["attackable_range"];
		clone->Hp = monster["hp"];
		clone->AttackPower = monster["attack_power"];
		clone->Armor = monster["armor"];
		clone->Speed = monster["speed"];
		if (_monsters.find(clone->MonsterId) != _monsters.end())
			CRASH("레전드 ID 중복 발생");
		_monsters[clone->MonsterId] = clone;
	}
}

shared_ptr<Monster> MonsterManager::Clone(uint8 monsterId)
{
	ASSERT_CRASH((_monsters.find(monsterId) != _monsters.end()));
	auto monster = _monsters[monsterId];

	auto clone = GPoolManager->Pop<Monster>();
	clone->MonsterId = monster->MonsterId;
	clone->AttackableRange = monster->AttackableRange;
	clone->Hp = monster->Hp;
	clone->AttackPower = monster->AttackPower;
	clone->Armor = monster->Armor;
	clone->Speed = monster->Speed;
	return shared_ptr<Monster>(clone);
}
