#pragma once
#include "Session.h"

class PacketHandler
{
public:
	static void C_TestHandler(PacketSession* session, ByteRef& buffer);
};