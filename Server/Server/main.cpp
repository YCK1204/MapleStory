#include "pch.h"
#include "ThreadManager.h"
#include "Listener.h"

int main()
{
	Listener listener;

	{
		sockaddr_in dbAddr;
		::memset(&dbAddr, 0, sizeof(dbAddr));
		dbAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		dbAddr.sin_port = htons(8080);
		dbAddr.sin_family = AF_INET;
		Manager::Session.dbSession->Connect(dbAddr);
	}

	GPoolManager->CreatePool<ClientSession>(500);
	{
		sockaddr_in clientAddr;
		::memset(&clientAddr, 0, sizeof(clientAddr));
		clientAddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		clientAddr.sin_port = htons(8087);
		clientAddr.sin_family = AF_INET;
		bool ret = listener.StartAccept(clientAddr, []() { return reinterpret_cast<PacketSession*>(GPoolManager->Pop<ClientSession>()); });
	}
	for (int i = 0; i < 2; i++)
	{
		GThreadManager->Launch([]() { GIocpCore->Dispatch(); });
	}
	cout << "Listening...\n";
	while (true)
	{
		this_thread::yield();
	}
}