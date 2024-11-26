#pragma once

class SocketUtils
{
public:
	static LPFN_CONNECTEX connectEx;
	static LPFN_DISCONNECTEX disconnectEx;
	static LPFN_ACCEPTEX acceptEx;
public:
	static void Init();
	static void Clear();
	static SOCKET CreateSocket();
	static bool Bind(SOCKET socket, sockaddr_in& addr);
	static bool BindAnyAddress(SOCKET socket, uint16 port);
	static bool BindWindowsFunction(SOCKET socket, GUID guid, LPVOID* fn);

	static bool SetLinger(SOCKET socket, uint16 onoff, uint16 linger);
	static bool SetReuseAddress(SOCKET socket, bool flag);
	static bool SetRecvBufferSize(SOCKET socket, int32 size);
	static bool SetSendBufferSize(SOCKET socket, int32 size);
	static bool SetTcpNoDelay(SOCKET socket, bool flag);
	static bool SetUpdateAcceptSocket(SOCKET socket, SOCKET listenSocket);

	static bool Listen(SOCKET socket, int32 backlog);
	static void Close(SOCKET socket);
};

template<typename T>
inline bool SetSockOpt(SOCKET socket, int32 level, int32 optName, T optVal)
{
	return SOCKET_ERROR != setsockopt(socket, level, optName, reinterpret_cast<const char*>(&optVal), sizeof(optVal));
}
