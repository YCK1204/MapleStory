#include "pch.h"
#include "GameRoom.h"

uint64 GameRoom::GenerateId(const ObjectType& type)
{
	uint64 id;
	{
		WRITE_LOCK;
		id = _curId++;
	}
	return id & ((uint64)type << 56);
}

ObjectType GameRoom::ExtractObjType(GameObjectRef& obj)
{
	return ObjectType(obj->Id >> 56);
}

const uint8 GameRoom::GetServerId() const
{
	return ((_roomId & SERVER_MASK) >> 24);
}

const uint8 GameRoom::GetChannelId() const
{
	return ((_roomId & CHANNEL_MASK) >> 16);
}

const uint8 GameRoom::GetMapId() const
{
	return ((_roomId & MAP_MASK) >> 16);
}

GameRoom::GameRoom(uint32 roomId)
{
	_roomId = roomId;
}

GameRoom::~GameRoom()
{
}

GameObject* GameRoom::Find(uint64& id)
{
	GameObject* go = nullptr;
	READ_LOCK;
	auto it = _objects.find(id);
	if (it != _objects.end())
		go = it->second.get();
	return go;
}

void GameRoom::Remove(uint64& id)
{
	WRITE_LOCK;

	_objects.erase(id);
}

void GameRoom::Push(GameObjectRef& go)
{
	uint64 id = GenerateId(go->Type);
	WRITE_LOCK;
	_objects[id] = go;
}

void GameRoom::Push(GameObject* go)
{
	GameObjectRef ref = shared_ptr<GameObject>(go);
	Push(ref);
}

void GameRoom::Broadcast(SendBufferRef pkt)
{
	for (auto it = _objects.begin(); it != _objects.end(); it++)
	{
		auto type = ExtractObjType(it->second);
		if (type != ObjectType::PLAYER)
			continue;
		Player* player = static_cast<Player*>(it->second.get());
		if (player == nullptr)
			continue;
		ClientRef clientSession = player->Session.lock();
		if (clientSession == nullptr)
			continue;
		clientSession->Send(pkt);
	}
}
