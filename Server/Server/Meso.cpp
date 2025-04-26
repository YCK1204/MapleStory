#include "pch.h"
#include "Meso.h"
#include "RandomNumberGenerator.h"

Meso::Meso()
{
	ItemType = ItemType::COIN;
	Money = RandomNumberGenerator::getRandomInt(1, 100);
}