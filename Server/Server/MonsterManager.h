#pragma once

class MonsterManager
{
private:
	map<uint8, shared_ptr<Monster>> _monsters;
	static MonsterManager* _instance;
public:
	static MonsterManager& Instance();
	void Init(json& monsterJson);
	shared_ptr<Monster> Clone(uint8 monsterId);
};
