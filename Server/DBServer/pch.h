#pragma once

#define WIN32_LEAN_AND_MEAN // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#pragma comment(lib, "..\\Lib\\ServerCore.lib")
#pragma comment(lib, "libmySQL.lib")

#include "Protocol_generated.h"

using namespace flatbuffers;
#include "CorePch.h"
#include "Manager.h"