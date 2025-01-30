#pragma once

#include "GameObject.h"

class GameRoom
{
private:
	static const uint32 SERVER_MASK = 0xFFFF'0000'0000'0000;
	static const uint32 CHANNEL_MASK = 0x0000'FFFF'0000'0000;
	static const uint32 MAP_MASK = 0x0000'0000'0000'FFFF;

private:
	USE_LOCK;
	map<uint64, GameObjectRef> _objects;
	uint32 _roomId;
	uint64 _curId = 0;
private:
	uint64 GenerateId(const ObjectType& type);
	ObjectType ExtractObjType(GameObjectRef& obj);

public:
	const uint8 GetServerId() const;
	const uint8 GetChannelId() const;
	const uint8 GetMapId() const;
public:
	GameRoom() = delete;
	GameRoom(uint32 roomId);
	~GameRoom();
	GameObject* Find(uint64& id);
	void Remove(uint64& id);
	void Push(GameObjectRef& go);
	void Push(GameObject* go);
	void Broadcast(SendBufferRef pkt);
};