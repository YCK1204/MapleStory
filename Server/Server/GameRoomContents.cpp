#include "pch.h"
#include "GameRoom.h"
#include "RandomNumberGenerator.h"

void GameRoom::GenMonster()
{
	if (_spawnInfo.size() == 0)
		return;

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
			clone->Map = _map;
			clone->Room = shared_ptr<GameRoom>(this, [](GameRoom* room) {});
			auto go = static_pointer_cast<GameObject>(clone);
			Push(go);

			clone->Pos->X = RandomNumberGenerator::getRandomInt(key->RangeX[0], key->RangeX[1]);
			clone->Pos->Y = key->Y;
			clone->SpawnInfo = spawnInfo.first;
			value.insert(clone);
		}
	}
}

const bool GameRoom::CanPort(uint8 id) const
{
	auto portal = find_if(portals.begin(), portals.end(), [id](uint8 portalId) { return portalId == id; });
	return portal != portals.end();
}

void GameRoom::BroadcastMonsterInfo()
{
	if (_objects.size() == 0)
		return;
	FlatBufferBuilder builder;
	auto infos = GenMonsterInfoDetails(builder);
	auto data = CreateSC_MonsterInfos(builder, infos);
	auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType::PacketType_SC_MonsterInfos);
	Broadcast(pkt);
}

void GameRoom::CheckItemLifetime()
{
	if (_objects.size() == 0)
		return;
	vector<uint64> deletedItems;
	deletedItems.reserve(_objects.size());

	for (auto it = _objects.begin(); it != _objects.end();)
	{
		if (it->second->Type != ObjectType::ITEM)
		{
			it++;
			continue;
		}
		auto item = reinterpret_pointer_cast<Item>(it->second);
		if (item->CreatedTime + item->LifeTime < GetTickCount64())
		{
			deletedItems.push_back(item->Id);
			it = _objects.erase(it);
		}
		else
		{
			it++;
		}
	}

	if (deletedItems.size())
	{
		FlatBufferBuilder builder;

		auto itemVector = builder.CreateVector(deletedItems);
		auto data = CreateSC_DespawnItem(builder, itemVector);
		auto pkt = Manager::Packet.CreatePacket(data, builder, PacketType_SC_DespawnItem);
		Broadcast(pkt);
		cout << "아이템 없어짐 : " << deletedItems.size() << endl;
		// 아이템 없어짐 패킷 브로드 캐스트
	}
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
	auto tick = GetTickCount64();
	if (LastSpawnUpdate + SpawnUpdateTickTime < tick)
	{
		LastSpawnUpdate = tick;
		PushJob([this]() { GenMonster(); });
	}
	if (LastMonsterUpdate + MonsterUpdateTickTime < tick)
	{
		LastMonsterUpdate = tick;
		PushJob([this]() { UpdateMonster(); });
	}
	if (LastMonsterInfoBroadcast + MonsterInfoBroadcastTickTime < tick)
	{
		LastMonsterInfoBroadcast = tick;
		PushJob([this]() { BroadcastMonsterInfo(); });
	}
	if (LastCheckItemLifetime + CheckItemLifeTicktime < tick)
	{
		LastCheckItemLifetime = tick;
		PushJob([this]() { CheckItemLifetime(); });
	}
}

void GameRoom::UpdateMonster()
{
	for (auto& [spawnInfo, monsterSet] : _spawnInfo)
	{
		for (auto it = monsterSet.begin(); it != monsterSet.end();)
		{
			auto monster = static_pointer_cast<Monster>(*it);

			monster->Update();
			if (monster->GetState() == MonsterState::MonsterState_Die) {
				RemoveObject(monster->Id);
				it = monsterSet.erase(it);
			}
			else {
				++it;
			}
		}
	}
}