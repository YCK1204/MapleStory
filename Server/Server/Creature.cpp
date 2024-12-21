#include "pch.h"
#include "Creature.h"

const bool Creature::IsAlive() const
{
    return (Hp > 0);
}

Creature::~Creature()
{
}

void Creature::EnterRoom(uint32& roomId)
{
    GameRoom* room = Manager::Room.Find(roomId);
    if (room)
        room->Push(this);
}

void Creature::LeaveRoom(uint32& roomId)
{
    GameRoom* room = Manager::Room.Find(roomId);
    if (room)
        room->Remove(this->Id);
}
