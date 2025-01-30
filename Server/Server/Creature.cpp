#include "pch.h"
#include "Creature.h"

const bool Creature::IsAlive() const
{
    return (Hp > 0);
}

Creature::~Creature()
{
}


