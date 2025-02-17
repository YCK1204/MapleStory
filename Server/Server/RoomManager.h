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
	uint8 _curId = 0;
	
public:
	void Init(json& j, uint8 maxMapId);
	GameRoomRef Find(uint32& id);
	uint32 GenerateRoomId(uint8 serverId, uint8 ChannelId, uint8 roomId);
};