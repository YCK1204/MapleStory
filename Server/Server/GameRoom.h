#pragma once

#include "GameObject.h"
#include "Job.h"

class GameRoom
{
private:
	static const uint32 SERVER_MASK = 0xFFFF'0000'0000'0000;
	static const uint32 CHANNEL_MASK = 0x0000'FFFF'0000'0000;
	static const uint32 MAP_MASK = 0x0000'0000'FFFF'FFFF;
	
private:
	USE_LOCK;
	map<uint64, GameObjectRef> _objects;
	map<uint64, PlayerRef> _players;
	uint32 _roomId;
	uint64 _curId = 0;
	queue<JobRef> _jobQueue;
private:
	uint64 GenerateId(const ObjectType& type);

public:
	const uint8 GetServerId() const;
	const uint8 GetChannelId() const;
	const uint8 GetMapId() const;
	Offset<Vector<Offset<PlayerInfo>>> GetPlayerInfos(FlatBufferBuilder& builder);
	Offset<Vector<Offset<MonsterInfo>>> GetMonsterInfos(FlatBufferBuilder& builder);
public:
	GameRoom() = delete;
	GameRoom(uint32 roomId);
	~GameRoom();
	GameObject* Find(uint64& id);
	void Remove(uint64& id);
	void Remove(PlayerRef player);
	void Push(GameObjectRef& go);
	void Push(GameObject* go);
	void Push(PlayerRef player);
	void Broadcast(SendBufferRef pkt, PlayerRef exception = nullptr);
	void PushJob(JobRef job);
	template <typename _Ty1>
	void PushJob(function<void(_Ty1)> func, _Ty1 t1);
	template <typename _Ty1, typename _Ty2>
	void PushJob(function<void(_Ty1, _Ty2)> func, _Ty1 t1, _Ty2 t2);
	template <typename _Ty1, typename _Ty2, typename _Ty3>
	void PushJob(function<void(_Ty1, _Ty2, _Ty3)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3);
	void Update();
};

template<typename _Ty1>
inline void GameRoom::PushJob(function<void(_Ty1)> func, _Ty1 t1)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job1<_Ty1>>(func, t1));
	PushJob(jobRef);
}

template <typename _Ty1, typename _Ty2>
inline void GameRoom::PushJob(function<void(_Ty1, _Ty2)> func, _Ty1 t1, _Ty2 t2)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job2<_Ty1, _Ty2>>(func, t1, t2));
	PushJob(jobRef);
}

template <typename _Ty1, typename _Ty2, typename _Ty3>
inline void GameRoom::PushJob(function<void(_Ty1, _Ty2, _Ty3)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job3<_Ty1, _Ty2, _Ty3>>(func, t1, t2, t3));
	PushJob(jobRef);
}
