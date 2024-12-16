#pragma once

#include "GameObject.h"

class GameRoom
{
private:
	USE_LOCK;
	map<uint64, GameObjectRef> _objects;
	uint64 _curId = 0;
private:
	uint64 GenerateId(const ObjectType& type);
public:
	GameRoom();
	~GameRoom();
	GameObject* Find(uint64& id);
	void Remove(uint64& id);
	void Push(GameObjectRef& go);
	void Push(GameObject* go);
};