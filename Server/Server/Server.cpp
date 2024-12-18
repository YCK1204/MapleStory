#include "pch.h"
#include "Server.h"

Server::Server()
{
}

Server::~Server()
{
}

void Server::Init(json& j)
{
	// 서버의 정보 초기화
	{
		ASSERT_CRASH((j.find("name") != j.end()));
		ASSERT_CRASH((j.find("id") != j.end()));
		ASSERT_CRASH((j.find("max_user_count") != j.end()));

		_name = j["name"];
		_maxUserCount = j["max_user_count"];
		_id = j["id"];
	}

	// 채널들 초기화
	for (auto c : j["channels"])
	{
		uint8 channelId = c["id"];

		ASSERT_CRASH((_channels.find(channelId) == _channels.end()));

		ChannelRef channel = shared_ptr<Channel>(new Channel());
		_channels[channelId] = channel;
		channel->Init(c);
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

const uint16 Server::GetMaxUserCount() const {
	return _maxUserCount;
}