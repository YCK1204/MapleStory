#pragma once

#include "GameRoom.h"

class RoomManager
{
private:
	static RoomManager* _instance;
public:
	static RoomManager& Instance();

private:
	map<uint8, GameRoomRef> _rooms;
	uint8 _curId = 0;
	
public:
	void Init();
	GameRoom* Find(uint8& id);
};