#pragma once
#include "IocpCore.h"
#include <functional>
#include "Session.h"

class AcceptEvent;


class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();
public:
	bool StartAccept(sockaddr_in& addr, function<PacketSession*()> sessionFactory);
	void CloseSocket();
public:
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes) override;

private:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);
	function<Session*()> _sessionFactory;
protected:
	SOCKET _socket = INVALID_SOCKET;
	vector<AcceptEvent*> _acceptEvents;
};