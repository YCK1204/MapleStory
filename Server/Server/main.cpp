#include "pch.h"

int main()
{
	Manager::Init();

	while (true)
	{
		Manager::Update();
	}

	return 0;
}