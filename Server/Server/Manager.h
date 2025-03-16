#pragma once

#include "PacketManager.h"
#include "SessionManager.h"
#include "ServerManager.h"
#include "DataManager.h"

#ifndef COMMON_DATA_PATH
	#define COMMON_DATA_PATH "../../Data/"
#endif

class Manager
{
public:
	static PacketManager& Packet;
	static SessionManager& Session;
	static ServerManager& Server;
	static DataManager& Data;
public:
	static void Init();
	static void Update();
};