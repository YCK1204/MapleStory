#pragma once
#include "Session.h"

class PacketHandler
{
public:
	static void D_LoginHandler(PacketSession* session, ByteRef& buffer);
};