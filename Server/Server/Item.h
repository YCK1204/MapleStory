#pragma once

#include "GameObject.h"

enum class ItemType {
	COIN,
	ARMOR,
};

class Item : public GameObject
{
public:
	ItemType ItemType;
	const static uint16 LifeTime = 15000;
	uint64 CreatedTime = GetTickCount64();
	Item();
	virtual ~Item() {}
};