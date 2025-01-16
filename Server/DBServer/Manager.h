#pragma once

#include "PacketManager.h"
#include "DbManager.h"
#include "ServerSession.h"

#ifndef COMMON_JSON_PATH
	#define COMMON_JSON_PATH "../../Data/"
#endif

class Manager
{
private:
	Manager();

public:
	static ServerSession* session;
	static PacketManager& Packet;
	static DbManager& DB;

public:
	static void Init();
};