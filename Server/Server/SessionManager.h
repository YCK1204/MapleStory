#pragma once

#include "DbSession.h"
#include "ClientSession.h"

class SessionManager
{
private:
	static SessionManager* _instance;
	map<uint64, ClientRef> _sessions;
	uint64 _curId = 0;
	USE_LOCK;

private:
	SessionManager();

public:
	DbSession* dbSession = nullptr;
public:
	static SessionManager& Instance();
	~SessionManager();
public:
	void Push(ClientSession* session);
	ClientRef Find(uint64 id);
	void Remove(uint64 id);
};