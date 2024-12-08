#include "pch.h"
#include "Manager.h"
#include "Listener.h"
#include "ThreadPool.h"

PacketManager& Manager::Packet = PacketManager::Instance();
DbManager& Manager::DB = DbManager::Instance();

ServerSession* Manager::session = nullptr;

void Manager::Init()
{
	// -------------- DBManager Init ----------------
	{
		string connectionDataPath = COMMON_JSON_PATH + (string)"DBConnectionData.json";

		ifstream connection(connectionDataPath);
		bool s = connection.is_open();
		ASSERT_CRASH(connection.is_open());

		json j = json::parse(connection);

		ASSERT_CRASH((j.find("password") != j.end()));
		ASSERT_CRASH((j.find("uid") != j.end()));
		ASSERT_CRASH((j.find("db_name") != j.end()));
		ASSERT_CRASH((j.find("ip") != j.end()));
		ASSERT_CRASH((j.find("driver") != j.end()));
		Manager::DB.Init(j);
		connection.close();
	}
	// -------------- DBManager Init ----------------

	// -------------- GameServer Connecting ---------
	{
		Listener* listener = new Listener();

		string portJsonPath = COMMON_JSON_PATH + (string)"port.json";
		ifstream port(portJsonPath);

		ASSERT_CRASH(port.is_open());

		Manager::session = new ServerSession();
		json j = json::parse(port);
		port.close();

		sockaddr_in addr;
		::memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(::atoi(j["db_to_server"].dump().c_str()));
		GPoolManager->CreatePool<ServerSession>(1);
		GPoolManager->Push<ServerSession>(session);

		for (int32 i = 0; i < 3; i++)
			GThreadPool->EnqueueJob([]() { GIocpCore->Dispatch(); });
		for (int32 i = 0; i < 3; i++);
		listener->StartAccept(addr, [&]() { return GPoolManager->Pop<ServerSession>(); });
	}
	// -------------- GameServer Connecting ---------
}