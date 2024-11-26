#include "pch.h"
#include "Connector.h"
#include "SocketUtils.h"

void Connector::RegisterConnect()
{
	SOCKET socket = _session->GetSocket();
	SOCKADDR_IN& addr = _session->GetAddress();

	if (false == SocketUtils::connectEx(socket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in), nullptr, 0, nullptr, (LPOVERLAPPED)_connectEvent))
	{
		int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
			_session->HandleError(errCode);
	}
}

void Connector::ProcessConnect()
{
	_session->OnConnect();
}

void Connector::Start(sockaddr_in& addr, function<PacketSession* ()> sessionFactory)
{
	_session = sessionFactory();
	SOCKET socket = _session->GetSocket();
	auto port = addr.sin_port;
	::memset(&addr.sin_port, 0, sizeof(addr.sin_port));
	bool success = SocketUtils::Bind(socket, addr);
	addr.sin_port = port;
	_session->SetAddress(addr);
	success &= (GIocpCore->Register(_session) == true);

	_connectEvent = new ConnectEvent();
	_connectEvent->Init();
	_connectEvent->SetSession(_session);
	RegisterConnect();
}

Connector::Connector()
{
}

Connector::~Connector()
{
}

HANDLE Connector::GetHandle()
{
	return (HANDLE)_session->GetSocket();
}

void Connector::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	//ASSERT_CRASH((iocpEvent->GetType() == EventType::Connect));
	if (iocpEvent->GetType() != EventType::Connect)
		CRASH("");
	ProcessConnect();
}