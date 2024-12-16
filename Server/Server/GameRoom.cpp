#include "pch.h"
#include "GameRoom.h"

uint64 GameRoom::GenerateId(const ObjectType& type)
{
    WRITE_LOCK;
    return _curId++ & ((uint64)type << 56);
}

GameRoom::GameRoom()
{
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
    uint64 id = GenerateId(go->GetType());
    WRITE_LOCK;
    _objects[id] = go;
}

void GameRoom::Push(GameObject* go)
{
    GameObjectRef ref = shared_ptr<GameObject>(go);
    Push(ref);
}