#include "pch.h"
#include "GameRoom.h"
#include "RandomNumberGenerator.h"

uint64 GameRoom::GenerateId(const ObjectType& type)
{
	return ++_curId | ((uint64)type << 56);
}

void GameRoom::GenMonster()
{
	FlatBufferBuilder builder;
	vector<Offset<MonsterInfo>> monsterInfos;

	for (auto& spawnInfo : _spawnInfo)
	{
		auto& key = spawnInfo.first;
		auto& value = spawnInfo.second;

		if (value.size() >= key->spawnCount)
			continue;
		auto typeRange = key->monsterType.size() - 1;
		for (auto i = value.size(); i < key->spawnCount; i++)
		{
			auto ranInt = RandomNumberGenerator::getRandomInt(0, typeRange);
			auto typeId = key->monsterType[ranInt];
			auto clone = Manager::Data.MonsterClone(typeId);

			clone->Pos->X = RandomNumberGenerator::getRandomInt(key->RangeX[0], key->RangeX[1]);
			clone->Pos->Y = key->Y;
			clone->Id = GenerateId(clone->Type);
			auto pos = clone->GeneratePosition(builder);
			auto monsterInfo = CreateMonsterInfo(builder, clone->MonsterId, clone->Id, pos);
			monsterInfos.push_back(monsterInfo);
			value.insert(clone);
			_objects[clone->Id] = clone;
		}
	}
	if (monsterInfos.size() == 0)
		return;
	auto infos = builder.CreateVector(monsterInfos);
	auto data = CreateSC_MSpawn(builder, infos);
	auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_MSpawn);
	Broadcast(pkt);
}

const uint8 GameRoom::GetServerId() const
{
	return ((_roomId & SERVER_MASK) >> 24);
}

const uint8 GameRoom::GetChannelId() const
{
	return ((_roomId & CHANNEL_MASK) >> 16);
}

const uint8 GameRoom::GetMapId() const
{
	return ((_roomId & MAP_MASK));
}

// todo
// Character 관련 구조체 Set Get 유틸 전역 함수 추가 필요
Offset<Vector<Offset<PlayerInfo>>> GameRoom::GetPlayerInfos(FlatBufferBuilder& builder)
{
	vector<Offset<PlayerInfo>> infos;

	for (auto it = _players.begin(); it != _players.end(); it++)
	{
		PlayerRef player = it->second;
		auto prevInfo = player->GeneratePlayerInfo(builder);
		infos.push_back(prevInfo);
	}

	return builder.CreateVector(infos);
}

Offset<Vector<Offset<MonsterInfo>>> GameRoom::GetMonsterInfos(FlatBufferBuilder& builder)
{
	vector<Offset<MonsterInfo>> infos;

	for (auto it = _objects.begin(); it != _objects.end(); it++)
	{
		if (it->second->Type == ObjectType::MONSTER)
		{
			auto monster = reinterpret_cast<Monster*>(it->second.get());
			auto position = monster->GeneratePosition(builder);
			auto data = CreateMonsterInfo(builder, monster->MonsterId, monster->Id, position);
			infos.push_back(data);
		}
	}
	return builder.CreateVector(infos);
}

GameRoom::GameRoom(uint32 roomId)
{
	_roomId = roomId;
}

GameRoom::~GameRoom()
{
}

const bool GameRoom::CanPort(uint8 id) const
{
	auto portal = find_if(portals.begin(), portals.end(), [id](uint8 portalId) { return portalId == id; });
	return portal != portals.end();
}

GameObject* GameRoom::Find(uint64& id)
{
	GameObject* go = nullptr;
	auto it = _objects.find(id);
	if (it != _objects.end())
		go = it->second.get();
	return go;
}

void GameRoom::Remove(uint64& id)
{
	_objects.erase(id);
}

void GameRoom::Remove(PlayerRef player)
{
	_players.erase(player->Id);
	FlatBufferBuilder builder;
	auto data = CreateSC_PDespawn(builder, player->Id);
	auto packet = Manager::Packet.CreatePacket(data, builder, PacketType_SC_PDespawn);
	Broadcast(packet);
}

void GameRoom::Push(GameObjectRef& go)
{
	uint64 id = GenerateId(go->Type);
	_objects[id] = go;
	go->Id = id;
	// broadcast
}

void GameRoom::Push(GameObject* go)
{
	GameObjectRef ref = shared_ptr<GameObject>(go);
	Push(ref);
	// broadcast
}

void GameRoom::Push(PlayerRef player)
{
	uint64 id = GenerateId(player->Type);
	_players[id] = player;
	player->Id = id;
	FlatBufferBuilder builder;
	auto playerInfo = player->GeneratePlayerInfo(builder);

	auto data = CreateSC_PSpawn(builder, playerInfo);
	auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_PSpawn);
	Broadcast(pkt, player);
}

void GameRoom::Broadcast(SendBufferRef pkt, PlayerRef exception)
{
	for (auto pair : _players)
	{
		auto player = pair.second;
		if (player.get() != exception.get())
		{
			auto session = player->Session.lock();
			if (session != nullptr)
				session->Send(pkt);
		}
	}
}

void GameRoom::PushJob(function<void()> job, uint64 tick)
{
	auto j = make_shared<Job>(job, tick);
	PushJob(j);
}

void GameRoom::PushJob(JobRef job) {
	WRITE_LOCK;
	_jobQueue.push(job);
}

void GameRoom::Update() {
	WRITE_LOCK;
	while (_jobQueue.size() > 0)
	{
		auto job = _jobQueue.top();
		if (job->CanExecute() == false)
			break;
		job->Execute();
		_jobQueue.pop();
	}
	if (_players.size() == 0)
		return;
	if (LastSpawnUpdate + SpawnUpdateTickTime < GetTickCount64() && _spawnInfo.size() > 0)
	{
		LastSpawnUpdate = GetTickCount64();
		PushJob([this]() { GenMonster(); });
	}
}

void GameRoom::Init(json& room)
{
	minX = room["min_x"];
	minY = room["min_y"];
	maxX = room["max_x"];
	maxY = room["min_y"];
	InitPos[0] = room["init_pos"][0];
	InitPos[1] = room["init_pos"][1];
	if (room.find("portals") != room.end())
	{
		for (auto portal : room["portals"])
			portals.push_back(portal);
	}
	if (room.find("spawn_info") == room.end())
		return;
	auto sp = room["spawn_info"];

	for (auto i : sp)
	{
		auto info = make_shared<SpawnInfo>();
		ASSERT_CRASH((i.find("range_x") != i.end()));
		ASSERT_CRASH((i.find("y") != i.end()));
		ASSERT_CRASH((i.find("spawn_count") != i.end()));
		info->RangeX[0] = i["range_x"][0];
		info->RangeX[1] = i["range_x"][1];
		info->Y = i["y"];
		info->spawnCount = i["spawn_count"];
		auto types = i["monster_types"];
		info->monsterType.reserve(types.size());
		for (auto type : types) { info->monsterType.push_back(type); }
		_spawnInfo[info];
	}
}