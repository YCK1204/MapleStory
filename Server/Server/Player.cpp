#include "pch.h"
#include "Player.h"

Player::Player(ClientRef session)
{
	Session = session;
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

void Player::EnterRoom(uint8& roomId)
{
	GameRoom* room = Manager::Room.Find(roomId);
	if (room)
		room->Push(this);
}

void Player::LeaveRoom(uint8& roomId)
{
	GameRoom* room = Manager::Room.Find(roomId);
	if (room)
		room->Remove(this->Id);
}