#include "pch.h"
#include "GameRoom.h"

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

const uint32& GameRoom::GetRoomId() const
{
	return _roomId;
}

Offset<Vector<Offset<PlayerInfo>>> GameRoom::GenPlayerInfos(FlatBufferBuilder& builder)
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

Offset<Vector<Offset<MonsterInfo>>> GameRoom::GenMonsterInfos(FlatBufferBuilder& builder)
{
	vector<Offset<MonsterInfo>> infos;

	for (auto it = _objects.begin(); it != _objects.end(); it++)
	{
		if (it->second->Type == ObjectType::MONSTER)
		{
			auto monster = reinterpret_cast<Monster*>(it->second.get());
			if (monster->GetState() == MonsterState::MonsterState_Die)
				continue;
			auto info = monster->GenerateMonsterInfo(builder);
			infos.push_back(info);
		}
	}
	return builder.CreateVector(infos);
}

Offset<Vector<Offset<MonsterInfoDetail>>> GameRoom::GenMonsterInfoDetails(FlatBufferBuilder& builder)
{
	vector<Offset<MonsterInfoDetail>> infos;

	for (auto it = _objects.begin(); it != _objects.end(); it++)
	{
		if (it->second->Type == ObjectType::MONSTER)
		{
			auto monster = reinterpret_cast<Monster*>(it->second.get());
			if (monster->GetState() == MonsterState::MonsterState_Die)
				continue;
			auto info = monster->GenerateMonsterInfoDetail(builder);
			infos.push_back(info);
		}
	}
	return builder.CreateVector(infos);
}