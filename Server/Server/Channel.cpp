#include "pch.h"
#include "Channel.h"
#include "Player.h"

Channel::Channel()
{
}

Channel::~Channel()
{
}

void Channel::Init(json& j)
{
	ASSERT_CRASH((j.find("id") != j.end()));
	_id = j["id"];
}

const uint16& Channel::GetUserCount() const
{
	return _userCount;
}
