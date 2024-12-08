#pragma once

#include "PacketManager.h"
#include "SessionManager.h"

#ifndef COMMON_JSON_PATH
	#define COMMON_JSON_PATH "../../Data/"
#endif

class Manager
{
public:
	static PacketManager& Packet;
	static SessionManager& Session;
public:
	static void Init();
};