#include "pch.h"
#include "GameRoom.h"

GameRoom::GameRoom(uint32 roomId)
{
	_roomId = roomId;
}

GameRoom::~GameRoom()
{
}

uint64 GameRoom::GenerateId(const ObjectType& type)
{
	return ++_curId | ((uint64)type << 56);
}

GameObject* GameRoom::Find(uint64& id)
{
	auto it = _objects.find(id);
	if (it == _objects.end())
		return nullptr;
	return it->second.get();
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

void GameRoom::Init(json& room, shared_ptr<ifstream> map)
{
	InitPos[0] = room["init_pos"][0];
	InitPos[1] = room["init_pos"][1];
	if (room.find("portals") != room.end())
	{
		for (auto portal : room["portals"])
			portals.push_back(portal);
	}
	_map = make_shared<Map>(*map.get());
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
	GenMonster();
}