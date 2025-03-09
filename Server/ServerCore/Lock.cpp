#include "pch.h"
#include "Lock.h"

void Lock::WriteLock()
{
	if (LThreadId == _writeLockFlag.load())
	{
		_writeCount.fetch_add(1);
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
				_writeCount.fetch_add(1);
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
	uint16 count = _writeCount.fetch_sub(1);
	if (count == 1)
		_writeLockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock()
{
	uint32 id = _writeLockFlag.load();
	if (id == EMPTY_FLAG || id == LThreadId)
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