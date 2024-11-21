#pragma once
#include "pch.h"

class IPool
{
public:
	virtual ~IPool() {}
	virtual void Init(int count, int maxSize) = 0;
	virtual void Clear() = 0;
};

template <typename T>
class Pool : public IPool
{
private:
	stack<T*> _poolStack;
	mutex _lock;
	uint16 _maxSize;
	uint16 _curSize;

public:
	Pool() {}
	~Pool() { Clear(); }
	void Init(int count, int maxSize) override
	{
		_maxSize = maxSize;
		_curSize = maxSize;
		for (int i = 0; i < count; i++)
		{
			T* obj = new T();
			_poolStack.push(obj);
		}
	}
	void Clear() override
	{
		auto size = _poolStack.size();
		for (int i = 0; i < size; i++)
		{
			T* obj = _poolStack.top();
			delete obj;
			_poolStack.pop();
		}
	}
	void Push(T* obj)
	{
		lock_guard<mutex> lock(_lock);

		_curSize++;
		_poolStack.push(obj);
	}
	T* Pop()
	{
		lock_guard<mutex> lock(_lock);

		if (_curSize <= 0)
			return nullptr;
		if (_poolStack.empty() == true)
			return new T();
		T* obj = _poolStack.top();
		_poolStack.pop();
		_curSize--;
		return obj;
	}
};

class PoolManager
{
private:
	unordered_map<string, IPool*> _pools;
	mutex _lock;
public:
	template <typename T>
	void CreatePool(int count = 5, int maxSize = 10)
	{
		if (maxSize < count)
			maxSize = count;
		string name = typeid(T).name();

		lock_guard<mutex> lock(_lock);
		{
			auto it = _pools.find(name);
			if (it != _pools.end())
				return;
			Pool<T>* pool = new Pool<T>();
			pool->Init(count, maxSize);
			_pools[name] = pool;
		}
	}
	template <typename T>
	void Push(T* obj)
	{
		string name = typeid(T).name();
		auto it = _pools.find(name);
		if (it == _pools.end())
		{
			delete obj;
			return;
		}
		Pool<T>* pool = static_cast<Pool<T>*>(it->second);
		pool->Push(obj);
	}
	template <typename T>
	T* Pop()
	{
		string name = typeid(T).name();
		auto it = _pools.find(name);
		if (it == _pools.end())
		{
			CreatePool<T>(5);
			it = _pools.find(name);
		}
		Pool<T>* pool = static_cast<Pool<T>*>(it->second);
		return pool->Pop();
	}
};