#pragma once

#include "Item.h"

class Meso : public Item
{
public:
	uint16 Money = 0;
	Meso();
	virtual ~Meso() {}
};