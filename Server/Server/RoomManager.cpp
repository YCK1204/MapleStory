#include "pch.h"
#include "RoomManager.h"

RoomManager* RoomManager::_instance = nullptr;

// [ server ] [ channel ] [       roomId       ]
uint32 RoomManager::GenerateRoomId(uint8 serverId, uint8 channelId, uint8 roomId)
{
	uint32 id = 0;

	id &= (serverId << 24);
	id &= (channelId << 16);
	id &= (roomId);

	return id;
}

RoomManager& RoomManager::Instance()
{
	if (_instance == nullptr)
		_instance = new RoomManager();
	return *_instance;
}

void RoomManager::Init(json& j, uint8 maxMapId)
{
	uint8 serverCnt = 0;

	for (auto& serverInfo : j["servers"])
	{
		uint8 channelCnt = 0;
		int32 channelCount = serverInfo["channel_count"];

		for (auto channelCnt = 0; channelCnt < channelCount; channelCnt++)
		{
			for (auto mapId = 1; mapId <= maxMapId; mapId++)
			{
				auto roomId = GenerateRoomId(serverCnt, channelCnt, mapId);
				auto room = make_shared<GameRoom>(roomId);

				_rooms[roomId] = room;
			}
		}
		serverCnt++;
	}
}

GameRoomRef RoomManager::Find(uint32& id)
{
	GameRoomRef room = nullptr;

	auto it = find_if(_rooms.begin(), _rooms.end(), [id](pair<uint32, GameRoomRef> val) { return (val.second->GetMapId() == id); });
	if (it != _rooms.end())
		room = it->second;
	return room;
}