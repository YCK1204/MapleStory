#include "pch.h"
#include "Listener.h"
#include "IocpEvent.h"
#include "SocketUtils.h"

Listener::~Listener()
{
	CloseSocket();
	for (auto e : _acceptEvents)
		delete e;
}

bool Listener::StartAccept(sockaddr_in& addr, function<PacketSession* ()> sessionFactory)
{
	_socket = SocketUtils::CreateSocket();

	bool success = true;
	success &= (_socket != INVALID_SOCKET);
	success &= (SocketUtils::SetReuseAddress(_socket, true) == true);
	success &= (SocketUtils::SetLinger(_socket, 0, 0) == true);
	success &= (SocketUtils::Bind(_socket, addr) == true);
	success &= (SocketUtils::Listen(_socket, 10) == true);
	success &= (GIocpCore->Register(this) == true);

	if (success == false)
		return false;

	_sessionFactory = sessionFactory;
	const int32 acceptCount = 1;
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = new AcceptEvent();
		_acceptEvents.push_back(acceptEvent);
		RegisterAccept(acceptEvent);
	}
	return true;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(_socket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH((iocpEvent->GetType() == EventType::Accept));
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	// 누수 관리 필요
	Session* session = nullptr;

	while (true)
	{
		session = _sessionFactory();
		if (session != nullptr)
			break;
		this_thread::yield();
	}
	acceptEvent->Init();
	acceptEvent->SetSession(session);

	DWORD bytes = 0;
	byte bytess[1000];
	if (false == SocketUtils::acceptEx(_socket, session->GetSocket(), &bytess, 0, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, OUT & bytes, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	Session* session = acceptEvent->GetSession();

	if (SOCKET_ERROR == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	sockaddr_in addr;
	int32 len = sizeof(addr);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<sockaddr*>(&addr), &len))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	session->SetAddress(addr);
	session->OnConnect();
	cout << "client connected!" << endl;
	RegisterAccept(acceptEvent);
}