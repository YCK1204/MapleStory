#include "pch.h"
#include "Lock.h"

void Lock::WriteLock()
{
	if (LThreadId == _writeLockFlag.load())
	{
		++_writeCount;
		return;
	}

	uint64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 i = 0; i < MAX_SPIN_COUNT; i++)
		{
			uint32 expected = EMPTY_FLAG;
			if (_writeLockFlag.compare_exchange_strong(OUT expected, LThreadId))
			{
				++_writeCount;
				goto NEXT;
			}
		}

		if (::GetTickCount64() - beginTick > TIMEOUT_TICK)
			CRASH("TIMEOUT");
		this_thread::yield();
	}

NEXT:
	beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 i = 0; i < MAX_SPIN_COUNT; i++)
		{
			if (_readLockFlag.load() == EMPTY_FLAG)
				return;
		}

		if (::GetTickCount64() - beginTick > TIMEOUT_TICK)
			CRASH("TIMEOUT");
		this_thread::yield();
	}
}

void Lock::WriteUnlock()
{
	if (--_writeCount == 0)
		_writeLockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	if (_writeLockFlag.load() == LThreadId)
	{
		_readLockFlag.fetch_add(1);
		return;
	}

	uint64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 i = 0; i < MAX_SPIN_COUNT; i++)
		{
			if (_writeLockFlag.load() == EMPTY_FLAG)
			{
				_readLockFlag.fetch_add(1);
				return;
			}
		}
		if (::GetTickCount64() - beginTick > TIMEOUT_TICK)
			CRASH("TIMEOUT");
		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if (_readLockFlag.fetch_sub(1) == 0)
		CRASH("MULTIPLE_UNLOCK");
}