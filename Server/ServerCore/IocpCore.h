#pragma once

class IocpObject
{
public:
	virtual HANDLE GetHandle() = 0;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes) = 0;
};

class IocpCore
{
private:
	HANDLE _iocpHandle;
public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() const { return _iocpHandle; }

	bool Register(class IocpObject* iocpObject);
	bool Dispatch(uint32 timeoutMs = INFINITE);
};