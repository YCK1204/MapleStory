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
	map<uint8, ServerRef>::iterator begin();
	map<uint8, ServerRef>::iterator end();
	void Init(json& j);
	ServerRef Find(uint8& id);
};