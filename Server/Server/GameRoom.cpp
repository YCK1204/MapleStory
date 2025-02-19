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

// todo
// Character 관련 구조체 Set Get 유틸 전역 함수 추가 필요
Offset<Vector<Offset<PlayerInfo>>> GameRoom::GetPlayerInfos(FlatBufferBuilder& builder)
{
	vector<Offset<PlayerInfo>> infos;

	WRITE_LOCK;
	for (auto it = _players.begin(); it != _players.end(); it++)
	{
		PlayerRef player = it->second;
		auto prevInfo = player->GeneratePlayerInfo(builder);
		infos.push_back(prevInfo);
	}

	return builder.CreateVector(infos);
}

Offset<Vector<Offset<MonsterInfo>>> GameRoom::GetMonsterInfos(FlatBufferBuilder& builder)
{
	vector<Offset<MonsterInfo>> infos;

	WRITE_LOCK;
	for (auto it = _objects.begin(); it != _objects.end(); it++)
	{

	}
	return builder.CreateVector(infos);
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

void GameRoom::Remove(PlayerRef& player)
{
	WRITE_LOCK;

	auto id = player->Id;
	_players.erase(player->Id);
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

void GameRoom::Push(PlayerRef& player)
{
	uint64 id = GenerateId(player->Type);
	WRITE_LOCK;
	_players[id] = player;
}

void GameRoom::Broadcast(SendBufferRef pkt, PlayerRef exception)
{
	for (auto it = _players.begin(); it != _players.end(); it++)
	{
		if (exception.get() == it->second.get())
			continue;
		ClientRef clientSession = static_cast<Player*>(it->second.get())->Session.lock();
		if (clientSession == nullptr)
			continue;
		clientSession->Send(pkt);
	}
}
