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
	void Init(json& servJ, json& roomInfo, vector<shared_ptr<ifstream>>& maps);
	void Update();
public:
	map<uint8, ChannelRef>::iterator begin();
	map<uint8, ChannelRef>::iterator end();
	Channel* FindChannel(uint8& id);
public:
	const uint8 GetId() const;
	const string GetName() const;
	const uint16 GetMaxUserCount() const;
	const vector<ChannelInfo> GetChannelInfos() const;
	const Offset<SC_ChannelInfo> GetChannelInfo(FlatBufferBuilder& bb) const;
};