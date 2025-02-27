#include "pch.h"
#include "Server.h"

Server::Server()
{
}

Server::~Server()
{
}

map<uint8, ChannelRef>::iterator Server::begin()
{
	return _channels.begin();
}

map<uint8, ChannelRef>::iterator Server::end()
{
	return _channels.end();
}

void Server::Init(json& j)
{
	// 서버의 정보 초기화
	{
		ASSERT_CRASH((j.find("name") != j.end()));
		ASSERT_CRASH((j.find("id") != j.end()));
		ASSERT_CRASH((j.find("max_user_count") != j.end()));
		ASSERT_CRASH((j.find("channel_count") != j.end()));

		_name = j["name"];
		_maxUserCount = j["max_user_count"];
		_id = j["id"];
	}

	// 채널들 초기화
	int32 channelCount = j["channel_count"];
	for (auto i = 1; i <= channelCount; i++)
	{
		ChannelRef channel = shared_ptr<Channel>(new Channel());
		_channels[i] = channel;
		channel->Init(_id, i, 1);
	}
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
