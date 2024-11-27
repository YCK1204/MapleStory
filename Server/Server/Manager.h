#pragma once

#include "PacketManager.h"
#include "SessionManager.h"

class Manager
{
public:
	static PacketManager& Packet;
	static SessionManager& Session;
public:
	Manager();
};