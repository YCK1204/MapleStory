#pragma once

#include "PacketManager.h"
#include "SessionManager.h"
#include "RoomManager.h"
#include "ServerManager.h"

#ifndef COMMON_JSON_PATH
	#define COMMON_JSON_PATH "../../Data/"
#endif

class Manager
{
public:
	static PacketManager& Packet;
	static SessionManager& Session;
	static RoomManager& Room;
	static ServerManager& Server;
public:
	static void Init();
};