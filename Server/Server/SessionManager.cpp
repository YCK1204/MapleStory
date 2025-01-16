#include "pch.h"
#include "SessionManager.h"
#include "SocketUtils.h"
#include "DbSession.h"
#include "ClientSession.h"

SessionManager* SessionManager::_instance = nullptr;

SessionManager& SessionManager::Instance()
{
	if (_instance == nullptr)
		_instance = new SessionManager();
	return *_instance;
}

SessionManager::~SessionManager()
{
}

void SessionManager::Push(ClientSession* session)
{
	WRITE_LOCK
	{
		session->SetSessionId(_curId);
		_sessions[_curId++] = shared_ptr<ClientSession>(session);
	}
}

ClientRef SessionManager::Find(uint32 id)
{
	ClientRef session = nullptr;
	READ_LOCK
	{
		auto it = _sessions.find(id);
		if (it != _sessions.end())
			session = it->second;
	}

	return session;
}

void SessionManager::Erase(uint32 id)
{
	WRITE_LOCK
	{
		_sessions.erase(id);
	}
}

SessionManager::SessionManager()
{
	SocketUtils::Init();
	dbSession = new DbSession();
}
