#pragma once

#include "DbSession.h"
#include "ClientSession.h"

class SessionManager
{
private:
	static SessionManager* _instance;
	vector<ClientSession*> _sessions;

private:
	SessionManager();

public:
	DbSession* dbSession = nullptr;
public:
	static SessionManager& Instance();
	~SessionManager();
public:

};