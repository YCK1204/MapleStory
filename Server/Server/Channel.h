#pragma once

#include "GameRoom.h"

class Channel
{
private:
	map<uint8, GameRoomRef> _rooms;
	uint8 _id;
	uint16 _userCount = 0;
public:
	Channel();
	~Channel();
public:
	void Init(uint8 serverId, uint8 channelId, json& rooms, vector<shared_ptr<ifstream>>& maps);
	const uint16& GetUserCount() const;
	GameRoomRef FindRoom(uint8 roomId);
	map<uint8, GameRoomRef>::iterator begin();
	map<uint8, GameRoomRef>::iterator end();
	void Update();
};