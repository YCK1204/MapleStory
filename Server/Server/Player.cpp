#include "pch.h"
#include "Player.h"

Player::Player()
{
	State = PlayerState::Lobby;
	Type = ObjectType::PLAYER;
}

Player::~Player()
{
}

void Player::TakeDamage(int32& damage)
{
	Hp -= damage;
	if (Hp <= 0)
		Hp = 0;
}
