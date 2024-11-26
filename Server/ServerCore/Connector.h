#pragma once

#include "Session.h"
#include <Windows.h>
#include <functional>

class Connector : public IocpObject
{
private:
	ConnectEvent* _connectEvent;
	Session* _session;

private:
	void RegisterConnect();
	void ProcessConnect();
public:
	void Start(sockaddr_in& addr, function<PacketSession* ()> sessionFactory);
	Connector();
	virtual ~Connector();

public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes) override;
};