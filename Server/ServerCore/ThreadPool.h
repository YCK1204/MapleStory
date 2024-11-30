#pragma once

#include <functional>

class ThreadPool
{
private:
	uint32 _numOfThreads;
	vector<thread> _threads;
	mutex _lock;
	queue<function<void()>> _jobQueue;
	condition_variable _cv;
	bool _allStop = false;
public:
	ThreadPool() = delete;
	ThreadPool(uint32 n);
	~ThreadPool();
	void WorkerThread();
	template <class F, class ...Args>
	future<typename invoke_result<F, Args...>::type> EnqueueJob(F&& f, Args&&... args) {
		using ReturnType = invoke_result<F, Args...>::type;

		auto job = make_shared<packaged_task<ReturnType()>>(bind(forward<F>(f), forward<Args...>(args)...));
		auto result = job->get_future();
		{
			lock_guard<mutex> lock(_lock);
			_jobQueue.push([job]() { (*job)(); });
		}
		_cv.notify_one();
		return result;
	}
};