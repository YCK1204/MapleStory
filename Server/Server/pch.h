#pragma once

#define WIN32_LEAN_AND_MEAN // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.


#pragma comment(lib, "..\\Lib\\ServerCore.lib")
#include <nlohmann/json.hpp>

#include "Protocol_generated.h"

using json = nlohmann::json;
using namespace flatbuffers;
#include "CorePch.h"
#include "Manager.h"