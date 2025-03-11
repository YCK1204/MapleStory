#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool(uint32 n)
{
	_numOfThreads = n;
	_threads.reserve(n);

	LThreadId = 1;
	for (uint32 i = 1; i <= n; i++)
	{
		_threads.emplace_back([this, i]() {
			LThreadId = i + 1;
			this->WorkerThread();
			});
	}
}

ThreadPool::~ThreadPool()
{
	_allStop = true;
	_cv.notify_all();
	for (uint32 i = 0; i < _numOfThreads; i++)
	{
		if (_threads[i].joinable())
			_threads[i].join();
	}
}

void ThreadPool::WorkerThread()
{
	while (true) {
		unique_lock lock(_lock);
		_cv.wait(lock, [this]() { return _allStop == true || !_jobQueue.empty(); });

		if (_allStop == true)
			return;
		auto job = move(_jobQueue.front());
		_jobQueue.pop();
		lock.unlock();

		job();
	}
}