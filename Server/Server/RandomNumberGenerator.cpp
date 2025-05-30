#include "pch.h"
#include "RandomNumberGenerator.h"

std::random_device RandomNumberGenerator::rd;
std::mt19937 RandomNumberGenerator::gen;

void RandomNumberGenerator::Init() { RandomNumberGenerator::gen = std::mt19937(rd()); }
int RandomNumberGenerator::getRandomInt(int min, int max) {
    uniform_int_distribution<int> dis(min, max);
    return dis(gen);
}

float RandomNumberGenerator::getRandomFloat(int min, int max, int p)
{
    int _min = min * p;
    int _max = max * p;
    return (float)getRandomInt(_min, _max) / (float)p;
}