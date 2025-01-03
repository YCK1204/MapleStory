#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#pragma comment(lib, "..\\Lib\\ServerCore.lib")

#include <nlohmann/json.hpp>
#include "Protocol_generated.h"
using json = nlohmann::json;
using namespace flatbuffers;
#include "CorePch.h"
#include "Manager.h"