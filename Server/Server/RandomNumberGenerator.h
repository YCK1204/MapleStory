#pragma once

#include <random>
class RandomNumberGenerator
{
private:
	static std::random_device rd;
	static std::mt19937 gen;
public:
	static void Init();
	static int getRandomInt(int min, int max);
	static float getRandomFloat(int min, int max, int p);
};
