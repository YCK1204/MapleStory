#include "pch.h"
#include "CoreGlobal.h"
#include "SocketUtils.h"
#include "IocpCore.h"
#include "PoolManager.h"
#include "ThreadPool.h"

IocpCore* GIocpCore = nullptr;
PoolManager* GPoolManager = nullptr;
ThreadPool* GThreadPool = nullptr;

class CoreGlobal
{
public:
	CoreGlobal() {
		const uint32 threadSize = 7;

		GIocpCore = new IocpCore();
		GPoolManager = new PoolManager();
		GThreadPool = new ThreadPool(threadSize);
		SocketUtils::Init();
	}
	~CoreGlobal() {
		delete GIocpCore;
		delete GPoolManager;
		SocketUtils::Clear();
	}
} GCoreGlobal;