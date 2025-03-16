#include "pch.h"
#include "Manager.h"
#include "Listener.h"
#include "ThreadPool.h"
#include <random>

PacketManager& Manager::Packet = PacketManager::Instance();
SessionManager& Manager::Session = SessionManager::Instance();
ServerManager& Manager::Server = ServerManager::Instance();
DataManager& Manager::Data = DataManager::Instance();

void Manager::Init() {
#pragma region DB 연결
	{
		string portJsonPath = COMMON_DATA_PATH + (string)"port.json";
		ifstream portJson(portJsonPath);

		ASSERT_CRASH(portJson.is_open());

		json j = json::parse(portJson);
		sockaddr_in dbAddr;
		::memset(&dbAddr, 0, sizeof(dbAddr));
		dbAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		dbAddr.sin_port = htons(::atoi(j["server_to_db"].dump().c_str()));
		dbAddr.sin_family = AF_INET;
		Manager::Session.dbSession->Connect(dbAddr);
#pragma endregion

#pragma region Listen
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
#pragma endregion

#pragma region Server And Room
	{
		// 난수 생성 엔진 초기화
		std::random_device rd;
		std::mt19937 gen(rd());

		string serverJsonPath = COMMON_DATA_PATH + (string)"server.json";
		ifstream serverJson(serverJsonPath);

		ASSERT_CRASH(serverJson.is_open());
		json j = json::parse(serverJson);

		Server.Init(j);
		serverJson.close();
	}
#pragma endregion

#pragma region Data
	{
		string dataJsonPath = COMMON_DATA_PATH + (string)"Data.json";
		ifstream dataJson(dataJsonPath);

		json j = json::parse(dataJson);
		Data.Init(j);
		dataJson.close();
	}
#pragma endregion
}

void Manager::Update()
{
	for (auto it = Server.begin(); it != Server.end(); it++)
		it->second->Update();
}