#include "pch.h"

int main() {
	Manager::Init();

	while (true)
		this_thread::sleep_for(0.1s);

	return 0;
}
