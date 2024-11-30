#pragma once

class Lock
{
public:
	enum : uint32
	{
		EMPTY_FLAG = 0x0000'0000,
		MAX_SPIN_COUNT = 5000,
		TIMEOUT_TICK = 10000,
	};
	void WriteLock();
	void WriteUnlock();
	void ReadLock();
	void ReadUnlock();
private:
	atomic<uint32> _writeLockFlag = 0;
	atomic<uint32> _readLockFlag = 0;
	uint16 _writeCount = 0;
};

class ReadLock
{
private:
	Lock& _lock;
public:
	ReadLock(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLock() { _lock.ReadUnlock(); }
};

class WriteLock
{
private:
	Lock& _lock;
public:
	WriteLock(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLock() { _lock.WriteUnlock(); }
};