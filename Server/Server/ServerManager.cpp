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

void ServerManager::Init(json& j)
{
	// 서버들 초기화

	ASSERT_CRASH((j.find("servers") != j.end()));

	json servers = j["servers"];
	for (auto s : servers)
	{
		uint8 serverId = s["id"];

		ASSERT_CRASH((_servers.find(serverId) == _servers.end()));
		ServerRef server = shared_ptr<Server>(new Server());;
		_servers[serverId] = server;
		server->Init(s);
	}
}

Server* ServerManager::Find(uint8& id)
{
	Server* server = nullptr;

	auto it = _servers.find(id);
	if (it != _servers.end())
		server = it->second.get();
	return server;
}

Offset<SC_ChannelInfo> ServerManager::GetChannelInfo(Server* server, FlatBufferBuilder& bb)
{
	uint8 serverId = server->GetId();

	auto channelInfoVec = server->GetChannelInfos();
	auto channelInfos = bb.CreateVectorOfStructs(channelInfoVec);

	return CreateSC_ChannelInfo(bb, serverId, channelInfos);
}