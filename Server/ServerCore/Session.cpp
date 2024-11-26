#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"

Session::Session() : _recvBuffer(bufferSize)
{
	_socket = SocketUtils::CreateSocket();

	_recvEvent = new RecvEvent();
	_sendEvent = new SendEvent();
	_disconnectEvent = new DisconnectEvent();
	_connectEvent = new ConnectEvent();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

SOCKET Session::GetSocket()
{
	return _socket;
}

sockaddr_in& Session::GetAddress()
{
	return _addr;
}

void Session::SetAddress(sockaddr_in& addr)
{
	_addr = addr;
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->GetType())
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	case EventType::Disconnect:
		ProcessDisconnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	}
}

void Session::RegisterConnect()
{
	SOCKADDR_IN& addr = GetAddress();

	if (false == SocketUtils::connectEx(_socket, reinterpret_cast<sockaddr*>(&addr), sizeof(sockaddr_in), nullptr, 0, nullptr, (LPOVERLAPPED)_connectEvent))
	{
		int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
			HandleError(errCode);
	}
}

void Session::RegisterRecv()
{
	if (_connected.load() == false)
		return;

	_recvEvent->Init();
	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer.GetWriteSegment());
	wsaBuf.len = _recvBuffer.GetWriteSize();

	SocketUtils::SetRecvBufferSize(_socket, _recvBuffer.GetWriteSize());
	try {
		DWORD numOfBytes = 0;
		DWORD flags = 0;
		if (SOCKET_ERROR == WSARecv(_socket, &wsaBuf, 1, OUT & numOfBytes, OUT & flags, _recvEvent, nullptr))
		{
			int32 errCode = ::WSAGetLastError();
			if (errCode != WSA_IO_PENDING)
			{
				HandleError(errCode);
			}
		}
	}
	catch (exception& e)
	{
		Disconnect();
	}
}

void Session::ProcessConnect()
{
	OnConnect();
}

void Session::ProcessRecv(int32 numOfBytes)
{
	if (numOfBytes == 0)
	{
		Disconnect();
		return;
	}

	if (_recvBuffer.OnWrite(numOfBytes) == false)
	{
		Disconnect();
		return;
	}

	int32 processLen = OnRecv(_recvBuffer.GetReadSegment(), _recvBuffer.GetReadSize());
	if (processLen > _recvBuffer.GetReadSize())
	{
		Disconnect();
		return;
	}
	if (_recvBuffer.OnRead(processLen) == false)
	{
		Disconnect();
		return;
	}
	_recvBuffer.Clear();
	RegisterRecv();
}

void Session::RegisterDisconnect()
{
	_disconnectEvent->Init();

	if (false == SocketUtils::disconnectEx(_socket, _disconnectEvent, TF_REUSE_SOCKET, 0))
	{
		int32 errCode = ::WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
			HandleError(errCode);
	}
}

void Session::RegisterSend()
{
	uint32 size = _sendQueue.size();

	auto& sendList = _sendEvent->_sendList;
	sendList.reserve(size);

	while (size > 0)
	{
		size--;
		sendList.push_back(shared_ptr<SendBuffer>(_sendQueue.front()));
		_sendQueue.pop();
	}

	size = sendList.size();
	vector<WSABUF> wsaBuf;
	wsaBuf.reserve(size);

	for (uint32 i = 0; i < size; i++)
	{
		auto& sendBuffer = sendList[i];

		WSABUF buf;
		buf.buf = reinterpret_cast<char*>(sendBuffer->operator byte * ());
		buf.len = sendBuffer->Size();
		wsaBuf.push_back(buf);
	}
	DWORD numOfBytes = 0;
	if (SOCKET_ERROR == WSASend(_socket, wsaBuf.data(), wsaBuf.size(), OUT & numOfBytes, 0, _sendEvent, nullptr))
	{
		int32 errCode = WSAGetLastError();
		if (errCode != WSA_IO_PENDING)
		{
			HandleError(errCode);
			sendList.clear();
		}
	}
}

void Session::ProcessSend(int32 numOfBytes)
{
	if (numOfBytes > 0)
	{
		_sendEvent->_sendList.clear();
		OnSend();
		if (_sendQueue.size() > 0)
			RegisterSend();
	}
	else
	{

	}
}

void Session::ProcessDisconnect()
{
	OnDisconnect();
}

void Session::Init()
{
	_connected.store(true);
	if (GIocpCore->Register(this) == false)
	{
		cout << "flase!\n";
	}
	_recvBuffer.Init();
}

void Session::HandleError(int32 errCode)
{
	switch (errCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect();
	default:
		// TODO : Log
		cout << "Handle Error : " << errCode << endl;
		break;
	}
}

void Session::Connect(sockaddr_in& addr)
{
	auto port = addr.sin_port;
	addr.sin_port = 0;
	bool success = SocketUtils::Bind(_socket, addr);
	addr.sin_port = port;
	SetAddress(addr);
	GIocpCore->Register(this);

	_connectEvent->Init();
	RegisterConnect();
}

void Session::Send(vector<byte>& bb)
{
	uint32 size = bb.size();

	SendBufferRef buffer = make_shared<SendBuffer>(bb.data(), size);
	Send(buffer);
}

void Session::Send(SendBufferRef& buffer)
{
	WRITE_LOCK;

	_sendQueue.push(buffer);
	if (_sendEvent->_sendList.size() == 0)
		RegisterSend();
}

void Session::Disconnect()
{
	if (_connected.exchange(false) == false)
		return;

	RegisterDisconnect();
}
