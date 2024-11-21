#pragma once

#include <mutex>
#include <cstddef>
#include <memory>
using namespace std;

using uint8 = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned long long;
using int8 = char;
using int16 = short;
using int32 = int;
using int64 = long long;

using LockGuard = lock_guard<mutex>;
using Mutex = mutex;
using SendBufferRef = shared_ptr<class SendBuffer>;