#pragma once
#include "Session.h"

class PacketHandler
{
public:
	static void SD_SignUpHandler(PacketSession* session, ByteRef& buffer);
	static void SD_SignInHandler(PacketSession* session, ByteRef& buffer);
};