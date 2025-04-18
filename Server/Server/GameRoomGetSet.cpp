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

	cout << "================================================================" << endl;
	for (auto it = _objects.begin(); it != _objects.end(); it++)
	{
		if (it->second->Type == ObjectType::MONSTER)
		{
			auto monster = reinterpret_cast<Monster*>(it->second.get());
			auto info = monster->GenerateMonsterInfo(builder);
			infos.push_back(info);
			cout << monster->Pos->X << ", " << monster->Pos->Y << endl;
		}
	}
	return builder.CreateVector(infos);
}