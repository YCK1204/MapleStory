#include "pch.h"
#include "CoreGlobal.h"
#include "ThreadManager.h"
#include "SocketUtils.h"
#include "IocpCore.h"
#include "PoolManager.h"


ThreadManager* GThreadManager = nullptr;
IocpCore* GIocpCore = nullptr;
PoolManager* GPoolManager = nullptr;

class CoreGlobal
{
public:
	CoreGlobal() {
		GThreadManager = new ThreadManager();
		GIocpCore = new IocpCore();
		GPoolManager = new PoolManager();
		SocketUtils::Init();
	}
	~CoreGlobal() {
		delete GThreadManager;
		delete GIocpCore;
		delete GPoolManager;
		SocketUtils::Clear();
	}
} GCoreGlobal;