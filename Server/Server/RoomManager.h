#pragma once

#include "GameRoom.h"

class RoomManager
{
private:
	static RoomManager* _instance;
public:
	static RoomManager& Instance();

private:
	map<uint32, GameRoomRef> _rooms;
	uint32 _curId = 0;
	
public:
	void Init();
	GameRoom* Find(uint32& id);
};