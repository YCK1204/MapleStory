#include "pch.h"
#include "Manager.h"
#include "Listener.h"
#include "ThreadPool.h"

PacketManager& Manager::Packet = PacketManager::Instance();
SessionManager& Manager::Session = SessionManager::Instance();

void Manager::Init() {
	string portJsonPath = COMMON_JSON_PATH + (string)"port.json";
	ifstream portJson(portJsonPath);

	ASSERT_CRASH(portJson.is_open());

	json j = json::parse(portJson);
	// ------------- db연결 --------------
	{
		sockaddr_in dbAddr;
		::memset(&dbAddr, 0, sizeof(dbAddr));
		dbAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		dbAddr.sin_port = htons(::atoi(j["server_to_db"].dump().c_str()));
		dbAddr.sin_family = AF_INET;
		Manager::Session.dbSession->Connect(dbAddr);
	}
	// ------------- db연결 --------------

	// ------------- Listen ---------------
	{
		GPoolManager->CreatePool<ClientSession>(500);
		Listener* listener = new Listener();

		sockaddr_in clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		clientAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		clientAddr.sin_port = htons(::atoi(j["server_to_client"].dump().c_str()));
		clientAddr.sin_family = AF_INET;
		listener->StartAccept(clientAddr, []() { return reinterpret_cast<PacketSession*>(GPoolManager->Pop<ClientSession>()); });
		for (int i = 0; i < 2; i++)
		{
			GThreadPool->EnqueueJob([]() { GIocpCore->Dispatch(); });
		}
		cout << "Listening...\n";
	}
	// ------------- Listen ---------------
}