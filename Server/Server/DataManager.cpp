#include "pch.h"
#include "DataManager.h"
#include "Monster.h"

DataManager* DataManager::_instance = nullptr;

DataManager& DataManager::Instance()
{
	if (_instance == nullptr)
		_instance = new DataManager();

	return *_instance;
}

void DataManager::Init(json& dataJson)
{
#pragma region Portal
	json portals = dataJson["portals"];
	for (auto portal : portals)
	{
		uint8 id = portal["id"];
		if (_portals.find(id) != _portals.end())
			CRASH("OVERLAPPED PORTAL ID");
		auto p = make_shared<Portal>();
		p->TargetId = portal["target_id"];
		p->SceneId = portal["scene_id"];
		p->X = portal["x"];
		p->Y = portal["y"];
		_portals[id] = p;
	}
#pragma endregion
#pragma region Monster
	json monsters = dataJson["monsters"];
	GPoolManager->CreatePool<Monster>(10, 1000);
	for (auto monster : monsters)
	{
		auto clone = make_shared<Monster>();

		clone->MonsterId = monster["id"];
		clone->AttackableRange[0] = monster["attackable_range"][0];
		clone->AttackableRange[1] = monster["attackable_range"][1];
		clone->Hp = monster["hp"];
		clone->AttackPower = monster["attack_power"];
		clone->Armor = monster["armor"];
		clone->Speed = monster["speed"];
		if (_monsters.find(clone->MonsterId) != _monsters.end())
			CRASH("레전드 ID 중복 발생");
		_monsters[clone->MonsterId] = clone;
	}
#pragma endregion
}

shared_ptr<Monster> DataManager::MonsterClone(uint8 monsterId)
{
	ASSERT_CRASH((_monsters.find(monsterId) != _monsters.end()));
	auto monster = _monsters[monsterId];

	auto clone = GPoolManager->Pop<Monster>();
	clone->MonsterId = monster->MonsterId;
	clone->AttackableRange[0] = monster->AttackableRange[0];
	clone->AttackableRange[1] = monster->AttackableRange[1];
	clone->Hp = monster->Hp;
	clone->AttackPower = monster->AttackPower;
	clone->Armor = monster->Armor;
	clone->Speed = monster->Speed;
	return shared_ptr<Monster>(clone);
}

DataManager::PortalRef DataManager::FindPortal(uint8& id) {
	 auto portal = _portals.find(id);

	 if (portal == _portals.end())
		 return nullptr;
	 return portal->second;
}
