#pragma once

#include "Server.h"

class ServerManager
{
private:
	static ServerManager* _instance;

private:
	map<uint8, ServerRef> _servers;

public:
	static ServerManager& Instance();
	~ServerManager();

public:
	void Init(json& j);
	Server* Find(uint8& id);
	Offset<SC_ChannelInfo> GetChannelInfo(Server* server, FlatBufferBuilder& bb);
};