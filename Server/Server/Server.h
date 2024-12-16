#pragma once

#include "Channel.h"

class Server
{
private:
	map<uint8, ChannelRef> _channels;
	string _name;
	uint16 _maxUserCount;
	uint8 _id;
public:
	Server();
	~Server();
public:
	void Init(json& j);
	Channel* FindChannel(uint8& id);
	const string GetName() const;
	const uint8 GetId() const;
	const vector<ChannelInfo> GetChannelInfos() const;
	const uint16 GetMaxUserCount() const;
};