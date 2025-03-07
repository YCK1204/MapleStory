#include "pch.h"
#include "Channel.h"
#include "Player.h"

Channel::Channel()
{
}

Channel::~Channel()
{
}

void Channel::Init(uint8 serverId, uint8 channelId, json& rooms)
{
	auto roomCount = rooms.size();
	_id = channelId;

	for (auto room : rooms)
	{
		uint8 id = room["id"];
		auto roomId = (serverId << 24) | (channelId << 16) | id;
		auto roomRef = make_shared<GameRoom>(roomId);
		_rooms[roomId] = roomRef;
		roomRef->Init(room);
	}
}

const uint16& Channel::GetUserCount() const
{
	return _userCount;
}

GameRoomRef Channel::FindRoom(uint8 roomId)
{
	auto room = find_if(_rooms.begin(), _rooms.end(), [roomId](const pair<uint8, GameRoomRef>& elem) { return elem.second->GetMapId() == roomId; });
	if (room == _rooms.end())
		return nullptr;
	return room->second;
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
