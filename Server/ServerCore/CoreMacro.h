#pragma once

#define in

#define CRASH(cause)						\
{											\
	int* crash = nullptr;					\
	__analysis_assume(crash != nullptr);	\
	*crash = 0xABCDEF;						\
}											\

#define ASSERT_CRASH(expr)					\
{											\
	if (!expr)								\
		CRASH("ASSERT_CRASH");				\
}

#define USE_MANY_LOCK(i) Lock _locks[i];
#define USE_LOCK USE_MANY_LOCK(1);
#define READ_MANY_LOCK(i) ReadLock lock(_locks[i]);
#define READ_LOCK READ_MANY_LOCK(0)
#define WRITE_MANY_LOCK(i) WriteLock lock(_locks[i]);
#define WRITE_LOCK WRITE_MANY_LOCK(0)