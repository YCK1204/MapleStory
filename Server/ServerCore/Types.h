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

template <class _Ty>
class BaseRef;

using Int8Ref = shared_ptr<BaseRef<int8>>;
using Int16Ref = shared_ptr<BaseRef<int16>>;
using Int32Ref = shared_ptr<BaseRef<int32>>;
using Int64Ref = shared_ptr<BaseRef<int64>>;
using UInt8Ref = shared_ptr<BaseRef<uint8>>;
using UInt16Ref = shared_ptr<BaseRef<uint16>>;
using UInt32Ref = shared_ptr<BaseRef<uint32>>;
using UInt64Ref = shared_ptr<BaseRef<uint64>>;
using ByteRef = shared_ptr<BaseRef<std::byte>>;