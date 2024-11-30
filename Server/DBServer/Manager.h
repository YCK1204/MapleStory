#pragma once

#include "PacketManager.h"
#include "DbManager.h"

#ifndef COMMON_JSON_PATH
	#define COMMON_JSON_PATH "../../Common/json/Data/"
#endif

class Manager
{
private:
	Manager();

private:

public:
	static PacketManager& Packet;
	static DbManager& DB;

public:
	static void Init();
};