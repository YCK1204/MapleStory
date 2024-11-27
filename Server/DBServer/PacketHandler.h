#pragma once
#include "Session.h"

class PacketHandler
{
public:
	static void S_LoginHandler(PacketSession* session, ByteRef& buffer);
};