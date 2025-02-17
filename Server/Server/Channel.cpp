#include "pch.h"
#include "Channel.h"
#include "Player.h"

Channel::Channel()
{
}

Channel::~Channel()
{
}

void Channel::Init(uint8 id)
{
	_id = id;
}

const uint16& Channel::GetUserCount() const
{
	return _userCount;
}
