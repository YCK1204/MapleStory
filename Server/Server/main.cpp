#include "pch.h"

int main()
{
	Manager::Init();

	while (true)
	{
		Manager::Update();
		this_thread::yield();
	}

	return 0;
}