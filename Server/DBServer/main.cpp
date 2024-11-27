#include "pch.h"
#include "ServerSession.h"
#include "ThreadManager.h"
#include "Listener.h"

int main() {
	Listener listener;
	ServerSession* session = new ServerSession();

	{
		sockaddr_in addr;
		::memset(&addr, 0, sizeof(addr));
		addr.sin_addr.s_addr = htonl(INADDR_ANY);
		addr.sin_family = AF_INET;
		addr.sin_port = htons(8080);
		GPoolManager->CreatePool<ServerSession>(1);
		GPoolManager->Push<ServerSession>(session);

		GThreadManager->Launch([]() { GIocpCore->Dispatch(); });
		listener.StartAccept(addr, [&]() { return GPoolManager->Pop<ServerSession>(); });
	}

	while (true)
		this_thread::sleep_for(0.1s);

	return 0;
}
