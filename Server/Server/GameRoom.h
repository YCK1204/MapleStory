#pragma once

#include "GameObject.h"
#include "Job.h"
#include "Monster.h"
#include <variant>

class GameRoom
{
private:
	static const uint32 SERVER_MASK = 0xFFFF'0000'0000'0000;
	static const uint32 CHANNEL_MASK = 0x0000'FFFF'0000'0000;
	static const uint32 MAP_MASK = 0x0000'0000'FFFF'FFFF;
	typedef struct {
		int32 RangeX[2];
		int32 Y;
		uint8 spawnCount;
		vector<uint8> monsterType;
	} SpawnInfo;
	typedef shared_ptr<SpawnInfo> SpawnInfoRef;
	typedef shared_ptr<Monster> MonsterRef;
private:
	USE_LOCK;
	map<uint64, GameObjectRef> _objects;
	map<uint64, PlayerRef> _players;
	uint64 _curId = 0;
	queue<JobRef> _jobQueue;
	const static uint64 SpawnUpdateTickTime = 3000;
	uint64 LastSpawnUpdate = GetTickCount64();

#pragma region MapInfo
private:
	uint32 _roomId;
	int32 minX;
	int32 maxX;
	int32 minY;
	int32 maxY;
	vector<uint8> portals;
	map<SpawnInfoRef, unordered_set<MonsterRef>> _spawnInfo;
public:
	float InitPos[2];
private:
	uint64 GenerateId(const ObjectType& type);
	void GenMonster();
#pragma endregion

#pragma region Getter
public:
	const uint8 GetServerId() const;
	const uint8 GetChannelId() const;
	const uint8 GetMapId() const;
	Offset<Vector<Offset<PlayerInfo>>> GetPlayerInfos(FlatBufferBuilder& builder);
	Offset<Vector<Offset<MonsterInfo>>> GetMonsterInfos(FlatBufferBuilder& builder);
#pragma endregion

#pragma region default constructor, destructor, Init
public:
	void Init(json& room);
	GameRoom() = delete;
	GameRoom(uint32 roomId);
	~GameRoom();
#pragma endregion

#pragma region Packet Handler
	const bool CanPort(uint8 id) const;
#pragma endregion


#pragma region Main Function
public:
	GameObject* Find(uint64& id);
	void Remove(uint64& id);
	void Remove(PlayerRef player);
	void Push(GameObjectRef& go);
	void Push(GameObject* go);
	void Push(PlayerRef player);
	void Broadcast(SendBufferRef pkt, PlayerRef exception = nullptr);
#pragma endregion

#pragma region Message Queue Util Functions
public:
	void PushJob(JobRef job);
	void PushJob(function<void()> job);
	template <typename _Ty1>
	void PushJob(function<void(_Ty1)> func, _Ty1 t1);
	template <typename _Ty1, typename _Ty2>
	void PushJob(function<void(_Ty1, _Ty2)> func, _Ty1 t1, _Ty2 t2);
	template <typename _Ty1, typename _Ty2, typename _Ty3>
	void PushJob(function<void(_Ty1, _Ty2, _Ty3)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3);
	template <typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4>
	void PushJob(function<void(_Ty1, _Ty2, _Ty3, _Ty4)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4);
	template <typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4, typename _Ty5>
	void PushJob(function<void(_Ty1, _Ty2, _Ty3, _Ty4, _Ty5)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4, _Ty5 t5);
	void Update();
#pragma endregion
};

#pragma region PushJob Definition
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

template<typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4>
inline void GameRoom::PushJob(function<void(_Ty1, _Ty2, _Ty3, _Ty4)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job4<_Ty1, _Ty2, _Ty3, _Ty4>>(func, t1, t2, t3, t4));
	PushJob(jobRef);
}

template<typename _Ty1, typename _Ty2, typename _Ty3, typename _Ty4, typename _Ty5>
inline void GameRoom::PushJob(function<void(_Ty1, _Ty2, _Ty3, _Ty4, _Ty5)> func, _Ty1 t1, _Ty2 t2, _Ty3 t3, _Ty4 t4, _Ty5 t5)
{
	auto jobRef = static_cast<JobRef>(make_shared<Job5<_Ty1, _Ty2, _Ty3, _Ty4, _Ty5>>(func, t1, t2, t3, t4, t5));
	PushJob(jobRef);
}
#pragma endregion