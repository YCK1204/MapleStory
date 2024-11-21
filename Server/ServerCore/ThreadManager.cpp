#include "pch.h"
#include "ThreadManager.h"
#include "pch.h"

ThreadManager::ThreadManager()
{
	InitTLS();
}

ThreadManager::~ThreadManager()
{
	Join();
}

void ThreadManager::Launch(function<void(void)> func)
{
	LockGuard lock(_lock);
	{
		_threads.push_back(thread([=]() {
			InitTLS();
			func();
			DestroyTLS();
			}));
	}
}

void ThreadManager::Join()
{
	for (auto& t : _threads)
	{
		if (t.joinable())
			t.join();
	}
}

void ThreadManager::InitTLS()
{
	auto id = this_thread::get_id();
	LThreadId = hash<thread::id>{}(id);
}

void ThreadManager::DestroyTLS()
{
}
