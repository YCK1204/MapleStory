#include "pch.h"
#include "RoomManager.h"

RoomManager* RoomManager::_instance = nullptr;

RoomManager& RoomManager::Instance()
{
	if (_instance == nullptr)
		_instance = new RoomManager();
	return *_instance;
}

void RoomManager::Init()
{
	// [8][16][24][32]
	// [Server][Channel][Island][RoomId]
}

GameRoom* RoomManager::Find(uint32& id)
{
	GameRoom* room = nullptr;

	auto it = _rooms.find(id);
	if (it != _rooms.end())
		room = it->second.get();
	return room;
}