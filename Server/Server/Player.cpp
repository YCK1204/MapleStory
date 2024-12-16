#include "pch.h"
#include "Player.h"

Player::Player()
{
	_state = CreatureState::LOBBY;
	_type = ObjectType::PLAYER;
}

Player::~Player()
{
}

void Player::SetMp(int32 mp)
{
	_mp = mp;
}

const int32 Player::GetMp() const
{
	return _mp;
}

void Player::TakeDamage(int32& damage)
{
	_hp -= damage;
	if (_hp <= 0)
		_hp = 0;
}
