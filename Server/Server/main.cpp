#include "pch.h"
#include "RandomNumberGenerator.h"

int main()
{
	RandomNumberGenerator::Init();
	Manager::Init();

	while (true)
	{
		Manager::Update();
		this_thread::yield();
	}

	return 0;
}