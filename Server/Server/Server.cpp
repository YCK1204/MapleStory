#include "pch.h"
#include "Server.h"

Server::Server()
{
}

Server::~Server()
{
}

void Server::Init(json& servJ, json& roomInfo)
{
	// 서버 정보 유효성 검사 및 초기화
	{
		ASSERT_CRASH((servJ.find("name") != servJ.end()));
		ASSERT_CRASH((servJ.find("id") != servJ.end()));
		ASSERT_CRASH((servJ.find("max_user_count") != servJ.end()));
		ASSERT_CRASH((servJ.find("channel_count") != servJ.end()));

		_name = servJ["name"];
		_maxUserCount = servJ["max_user_count"];
		_id = servJ["id"];
	}
	// 스폰 데이터 정보 유효성 검사
	{
		ASSERT_CRASH((roomInfo.find("rooms") != roomInfo.end()));
		json rooms = roomInfo["rooms"];
		for (auto room : rooms)
		{
			ASSERT_CRASH((room.find("id") != room.end()));
			ASSERT_CRASH((room.find("min_x") != room.end()));
			ASSERT_CRASH((room.find("max_x") != room.end()));
			ASSERT_CRASH((room.find("min_y") != room.end()));
			ASSERT_CRASH((room.find("max_y") != room.end()));
			/*ASSERT_CRASH((room.find("spawn_pos") != room.end()));
			json spawnPos = room["spawn_pos"];
			ASSERT_CRASH((spawnPos.find("range_x") != spawnPos.end()));
			ASSERT_CRASH((spawnPos.find("y") != spawnPos.end()));*/
		}
	}

	// 채널들 초기화
	int32 channelCount = servJ["channel_count"];
	json rooms = roomInfo["rooms"];
	for (auto i = 1; i <= channelCount; i++)
	{
		ChannelRef channel = shared_ptr<Channel>(new Channel());
		_channels[i] = channel;
		channel->Init(_id, i, rooms);
	}
}

map<uint8, ChannelRef>::iterator Server::begin()
{
	return _channels.begin();
}

map<uint8, ChannelRef>::iterator Server::end()
{
	return _channels.end();
}

Channel* Server::FindChannel(uint8& id)
{
	Channel* channel = nullptr;

	auto it = _channels.find(id);
	if (it != _channels.end())
		channel = it->second.get();
	return channel;
}

const string Server::GetName() const
{
	return _name;
}

const uint8 Server::GetId() const
{
	return _id;
}

const vector<ChannelInfo> Server::GetChannelInfos() const
{
	vector<ChannelInfo> channelInfos;

	channelInfos.reserve(_channels.size());

	for (auto channel : _channels)
	{
		auto info = ChannelInfo(channel.second->GetUserCount(), channel.first);
		channelInfos.push_back(info);
	}
	return channelInfos;
}

const Offset<SC_ChannelInfo> Server::GetChannelInfo(FlatBufferBuilder& bb) const
{
	auto channelInfoVec = GetChannelInfos();
	auto channelInfos = bb.CreateVectorOfStructs(channelInfoVec);

	return CreateSC_ChannelInfo(bb, _id, channelInfos);
}

const uint16 Server::GetMaxUserCount() const {
	return _maxUserCount;
}

void Server::Update()
{
	for (auto it = begin(); it != end(); it++)
		it->second->Update();
}
