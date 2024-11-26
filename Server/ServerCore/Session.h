#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "RecvBuffer.h"
#include "BitConverter.h"
#include "SendBuffer.h"

const static uint16 bufferSize = 65535;

class Session : public IocpObject
{

public:
	Session();
	virtual ~Session();

public:
	SOCKET GetSocket();
	sockaddr_in& GetAddress();
	void SetAddress(sockaddr_in& addr);
public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes) override;

protected:
	void RegisterRecv();
	void RegisterSend();
	void RegisterDisconnect();

private:
	void ProcessRecv(int32 numOfBytes);
	void ProcessDisconnect();
	void ProcessSend(int32 numOfBytes);

public:
	void Send(vector<byte>& bb);
	void Send(SendBufferRef& buffer);
	void Disconnect();

public:
	virtual void OnConnect() = 0;
	virtual void OnDisconnect() = 0;
	virtual void OnSend() = 0;
	virtual int32 OnRecv(byte* data, int32 size) = 0;
	void Init();

public:
	void HandleError(int32 errCode);
protected:
	USE_LOCK;
	queue<SendBufferRef> _sendQueue;
	SOCKET _socket = INVALID_SOCKET;
	sockaddr_in _addr = {};
private:
	RecvEvent _recvEvent;
	SendEvent _sendEvent;
	DisconnectEvent _disconnectEvent;
protected:
	atomic<bool> _connected = false;
	RecvBuffer _recvBuffer;
};

class PacketSession : public Session
{
private:
	static const int32 HeaderSize = 4;
public:
	PacketSession() {}
	virtual ~PacketSession() {}
	int32 OnRecv(byte* data, int32 size) override {
		int32 processLen = 0;

		while (true) {
			if (size < HeaderSize)
				break;
			uint16 packetSize = BitConverter::ToUInt16(_recvBuffer, processLen);
			if (packetSize < 0 || packetSize > size)
				break;
			OnRecvPacket(packetSize, data);
			processLen += packetSize;
			data = data + packetSize;
			size -= packetSize;
		}
		return processLen;
	}
	virtual void OnRecvPacket(int32 size, byte* data) = 0;
};

