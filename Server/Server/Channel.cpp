#include "pch.h"
#include "Channel.h"
#include "Player.h"

Channel::Channel()
{
}

Channel::~Channel()
{
}

void Channel::Init(uint8 serverId, uint8 channelId, uint8 roomCount)
{
	_id = channelId;

	for (auto i = 1; i <= roomCount; i++)
	{
		auto roomId = (serverId << 24) | (channelId << 16) | i;
		auto room = make_shared<GameRoom>(roomId);
		_rooms[roomId] = room;
	}
}

const uint16& Channel::GetUserCount() const
{
	return _userCount;
}

GameRoomRef Channel::FindRoom(uint8 roomId)
{
	GameRoomRef room = nullptr;
	for (auto it = _rooms.begin(); it != _rooms.end(); it++)
	{
		if (it->second->GetMapId() == roomId)
		{
			room = it->second;
			break;
		}
	}
	return room;
}

map<uint8, GameRoomRef>::iterator Channel::begin()
{
	return _rooms.begin();
}

map<uint8, GameRoomRef>::iterator Channel::end()
{
	return _rooms.end();
}

void Channel::Update()
{
	for (auto it = begin(); it != end(); it++)
		it->second->Update();
}
