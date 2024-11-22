#include "pch.h"
#include "ThreadManager.h"
#include "Listener.h"
#include "Session.h"
#include "SocketUtils.h"
#include "ClientSession.h"
#include "PoolManager.h"

int main()
{
	Listener listener;
	sockaddr_in addr;
	::memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	addr.sin_port = htons(8080);
	addr.sin_family = AF_INET;
	GPoolManager->CreatePool<ClientSession>(500);
	bool ret = listener.StartAccept(addr, []() { return reinterpret_cast<PacketSession*>(GPoolManager->Pop<ClientSession>()); });
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
