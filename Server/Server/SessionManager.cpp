#include "pch.h"
#include "SessionManager.h"
#include "SocketUtils.h"
#include "DbSession.h"
#include "ClientSession.h"
#include <functional>

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
	WRITE_LOCK;
	{
		session->SetSessionId(_curId);
		auto client = shared_ptr<ClientSession>(session, [](ClientSession* p) { 
			GPoolManager->Push<ClientSession>(p);
			if (p->Player->Room != nullptr)
				p->Player->Room->PushJob(std::bind(&GameRoom::RemovePlayer, p->Player->Room.get(), p->Player->Id));
			p->Player->Room = nullptr;
			});
		session->Player->Session = client;
		_sessions[_curId++] = client;
	}
}

ClientRef SessionManager::Find(uint64 id)
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

void SessionManager::Remove(uint64 id)
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
