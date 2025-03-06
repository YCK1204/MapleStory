#include "pch.h"
#include "ServerManager.h"

ServerManager* ServerManager::_instance = nullptr;

ServerManager& ServerManager::Instance()
{
	if (_instance == nullptr)
		_instance = new ServerManager();
	return *_instance;
}

ServerManager::~ServerManager()
{
	delete _instance;
}

map<uint8, ServerRef>::iterator ServerManager::begin()
{
	return _servers.begin();
}

map<uint8, ServerRef>::iterator ServerManager::end()
{
	return _servers.end();
}

void ServerManager::Init(json& j)
{
	// 서버들 초기화

	ASSERT_CRASH((j.find("servers") != j.end()));

	string roomInfoPath = COMMON_JSON_PATH + (string)"RoomInfo.json";

	ifstream roomInfo(roomInfoPath);
	ASSERT_CRASH(roomInfo.is_open());

	json roomInfoData = json::parse(roomInfo);
	json servers = j["servers"];
	for (auto s : servers)
	{
		uint8 serverId = s["id"];

		ASSERT_CRASH((_servers.find(serverId) == _servers.end()));
		ServerRef server = shared_ptr<Server>(new Server());;
		_servers[serverId] = server;
		server->Init(s, roomInfoData);
	}
}

ServerRef ServerManager::Find(uint8& id)
{
	ServerRef server = nullptr;

	auto it = _servers.find(id);
	if (it != _servers.end())
		server = it->second;
	return server;
}
