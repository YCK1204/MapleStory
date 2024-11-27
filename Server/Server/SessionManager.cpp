#include "pch.h"
#include "SessionManager.h"
#include "SocketUtils.h"

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

SessionManager::SessionManager()
{
	SocketUtils::Init();
	dbSession = new DbSession();
}
