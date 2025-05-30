#pragma once

#include "GameObject.h"
#include "Job.h"
#include "Monster.h"
#include "Meso.h"
#include "Map.h"
#include <variant>

struct SpawnInfo {
	int32 RangeX[2];
	int32 Y;
	uint8 spawnCount;
	vector<uint8> monsterType;
};

class GameRoom
{
private:
	static const uint32 SERVER_MASK = 0xFFFF'0000'0000'0000;
	static const uint32 CHANNEL_MASK = 0x0000'FFFF'0000'0000;
	static const uint32 MAP_MASK = 0x0000'0000'FFFF'FFFF;
	typedef shared_ptr<SpawnInfo> SpawnInfoRef;
	typedef shared_ptr<Monster> MonsterRef;
private:
	USE_LOCK;
	map<uint64, GameObjectRef> _objects;
	map<uint64, PlayerRef> _players;
	uint64 _curId = 0;
	priority_queue<JobRef, vector<JobRef>, greater<JobRef>> _jobQueue;
	const static uint16 SpawnUpdateTickTime = 5000;
	uint64 LastSpawnUpdate = GetTickCount64();
	const static uint16 MonsterUpdateTickTime = 250;
	uint64 LastMonsterUpdate = GetTickCount64();
	const static uint16 MonsterInfoBroadcastTickTime = 1500;
	uint64 LastMonsterInfoBroadcast = GetTickCount64();
	const static uint16 CheckItemLifeTicktime = 6'0000;
	uint64 LastCheckItemLifetime = GetTickCount64();

#pragma region MapInfo
private:
	uint32 _roomId;
	vector<uint8> portals;
	map<SpawnInfoRef, unordered_set<MonsterRef>> _spawnInfo;
	shared_ptr<Map> _map = nullptr;
public:
	float InitPos[2];
private:
	uint64 GenerateId(const ObjectType& type);
	void UpdateMonster();
#pragma endregion

#pragma region Getter
public:
	const uint8 GetServerId() const;
	const uint8 GetChannelId() const;
	const uint8 GetMapId() const;
	const uint32& GetRoomId() const;
	Offset<Vector<Offset<PlayerInfo>>> GenPlayerInfos(FlatBufferBuilder& builder);
	Offset<Vector<Offset<MonsterInfoDetail>>> GenMonsterInfoDetails(FlatBufferBuilder& builder);
	Offset<Vector<Offset<MonsterInfo>>> GenMonsterInfos(FlatBufferBuilder& builder);
	Offset<Vector<Offset<Coin>>> GenItemInfos(FlatBufferBuilder& builder);
#pragma endregion

#pragma region default constructor, destructor, Init
public:
	void Init(json& room, shared_ptr<ifstream> map);
	GameRoom() = delete;
	GameRoom(uint32 roomId);
	~GameRoom();
#pragma endregion

#pragma region Contents
	void GenMonster();
	const bool CanPort(uint8 id) const;
	void BroadcastMonsterInfo();
	void CheckItemLifetime();
#pragma endregion


#pragma region Main Function
public:
	GameObjectRef Find(uint64& id);
	GameObjectRef Find(const uint64& id);
	void RemoveObject(uint64& id);
	void RemoveObject(const uint64& id);
	void RemovePlayer(uint64& id);
	void Push(GameObjectRef& go);
	void Push(GameObject* go);
	void Push(PlayerRef player);
	void Broadcast(SendBufferRef pkt, PlayerRef exception = nullptr);
	void Update();
#pragma endregion

#pragma region Message Queue Util Functions
public:
	void PushJob(JobRef job);
	void PushJob(function<void()> job, uint64 tick = 0);
	template <typename _Ty1>
	void PushJob(function<void(_Ty1)> func, _Ty1 t1, uint64 tick = 0);
	template <typename _Ty1, typename _Ty2>
	void PushJob(function<void(_Ty1, _Ty2)> func, _Ty1 t1, _Ty2 t2, uint64 tick = 0);
	template <typename _Ty1, typename _Ty2, typename _Ty3>
	void PushJob(function<void(_Ty1, _Ty2, _Ty3)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, uint64 tick = 0);
	template <typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4>
	void PushJob(function<void(_Ty1, _Ty2, _Ty3, _Ty4)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4, uint64 tick = 0);
	template <typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4, typename _Ty5>
	void PushJob(function<void(_Ty1, _Ty2, _Ty3, _Ty4, _Ty5)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4, _Ty5 t5, uint64 tick = 0);
#pragma endregion
};

#pragma region PushJob Definition
template<typename _Ty1>
inline void GameRoom::PushJob(function<void(_Ty1)> func, _Ty1 t1, uint64 tick)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job1<_Ty1>>(func, t1, tick));
	PushJob(jobRef);
}

template <typename _Ty1, typename _Ty2>
inline void GameRoom::PushJob(function<void(_Ty1, _Ty2)> func, _Ty1 t1, _Ty2 t2, uint64 tick)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job2<_Ty1, _Ty2>>(func, t1, t2, tick));
	PushJob(jobRef);
}

template <typename _Ty1, typename _Ty2, typename _Ty3>
inline void GameRoom::PushJob(function<void(_Ty1, _Ty2, _Ty3)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, uint64 tick)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job3<_Ty1, _Ty2, _Ty3>>(func, t1, t2, t3, tick));
	PushJob(jobRef);
}

template<typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4>
inline void GameRoom::PushJob(function<void(_Ty1, _Ty2, _Ty3, _Ty4)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4, uint64 tick)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job4<_Ty1, _Ty2, _Ty3, _Ty4>>(func, t1, t2, t3, t4, tick));
	PushJob(jobRef);
}

template<typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4, typename _Ty5>
inline void GameRoom::PushJob(function<void(_Ty1, _Ty2, _Ty3, _Ty4, _Ty5)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4, _Ty5 t5, uint64 tick)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job5<_Ty1, _Ty2, _Ty3, _Ty4, _Ty5>>(func, t1, t2, t3, t4, t5, tick));
	PushJob(jobRef);
}
#pragma endregion