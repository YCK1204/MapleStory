#include "pch.h"
#include "SocketUtils.h"

LPFN_ACCEPTEX SocketUtils::acceptEx = nullptr;
LPFN_CONNECTEX SocketUtils::connectEx = nullptr;
LPFN_DISCONNECTEX SocketUtils::disconnectEx = nullptr;

void SocketUtils::Init()
{
	WSAData wsaData;
	ASSERT_CRASH(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0);

	SOCKET socket = CreateSocket();
	ASSERT_CRASH(BindWindowsFunction(socket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&connectEx)));
	ASSERT_CRASH(BindWindowsFunction(socket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&disconnectEx)));
	ASSERT_CRASH(BindWindowsFunction(socket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&acceptEx)));
	Close(socket);
}

void SocketUtils::Clear()
{
	WSACleanup();
}

SOCKET SocketUtils::CreateSocket()
{
	return ::WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
}

bool SocketUtils::Bind(SOCKET socket, sockaddr_in& addr)
{
	return SOCKET_ERROR != ::bind(socket, (const sockaddr*)&addr, sizeof(addr));
}

bool SocketUtils::BindAnyAddress(SOCKET socket, uint16 port)
{
	sockaddr_in addr;
	::memset(&addr, 0, sizeof(addr));

	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	return Bind(socket, addr);
}

bool SocketUtils::BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn)
{
	DWORD bytes = 0;
	return SOCKET_ERROR != WSAIoctl(socket, SIO_GET_EXTENSION_FUNCTION_POINTER, &guid, sizeof(guid), fn, sizeof(*fn), OUT &bytes, nullptr, nullptr);
}

bool SocketUtils::SetLinger(SOCKET socket, uint16 onoff, uint16 linger)
{
	LINGER option;
	option.l_onoff = onoff;
	option.l_linger = linger;
	return SetSockOpt(socket, SOL_SOCKET, SO_LINGER, option);
}

bool SocketUtils::SetReuseAddress(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_REUSEADDR, flag);
}

bool SocketUtils::SetRecvBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_RCVBUF, size);
}

bool SocketUtils::SetSendBufferSize(SOCKET socket, int32 size)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_SNDBUF, size);
}

bool SocketUtils::SetTcpNoDelay(SOCKET socket, bool flag)
{
	return SetSockOpt(socket, SOL_SOCKET, TCP_NODELAY, flag);
}

bool SocketUtils::SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket)
{
	return SetSockOpt(socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, listenSocket);
}

bool SocketUtils::Listen(SOCKET socket, int32 backlog)
{
	return SOCKET_ERROR != ::listen(socket, backlog);
}

void SocketUtils::Close(SOCKET socket)
{
	if (socket != INVALID_SOCKET)
		::closesocket(socket);
	socket = INVALID_SOCKET;
}
