#include "pch.h"
#include "Creature.h"

const bool Creature::IsAlive() const
{
    return false;
}

const int32& Creature::GetHp() const
{
    return _hp;
}

const PosInfo& Creature::GetPosInfo() const
{
    return _posInfo;
}

const CreatureState& Creature::GetState() const
{
    return _state;
}

void Creature::SetHp(int32& hp)
{
    _hp = hp;
}

void Creature::SetPosInfo(int16 x, int16 y)
{
    _posInfo.X = x;
    _posInfo.Y = y;
}

void Creature::SetPosInfo(PosInfo posInfo)
{
    _posInfo = posInfo;
}

void Creature::SetState(CreatureState state)
{
    _state = state;
}

Creature::Creature()
{
    _state = CreatureState::ALIVE;
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
        room->Remove(this->_id);
}
