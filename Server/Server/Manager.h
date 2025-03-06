#pragma once

#include "PacketManager.h"
#include "SessionManager.h"
#include "ServerManager.h"
#include "MonsterManager.h"

#ifndef COMMON_JSON_PATH
	#define COMMON_JSON_PATH "../../Data/"
#endif

class Manager
{
public:
	static PacketManager& Packet;
	static SessionManager& Session;
	static ServerManager& Server;
	static MonsterManager& Monster;
public:
	static void Init();
	static void Update();
};