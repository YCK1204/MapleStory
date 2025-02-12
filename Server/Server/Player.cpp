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

Offset<CharacterPreviewInfo> Player::GenerateCharPreviewInfo(FlatBufferBuilder& builder)
{
	auto ability = CreateCharacterAbility(
		builder,
		_ability->STR,
		_ability->DEX,
		_ability->INT,
		_ability->LUK);

	auto info = CreateCharacterPreviewInfoDirect(
		builder,
		_baseInfo->CharId,
		_baseInfo->CharType,
		_baseInfo->Level,
		_baseInfo->Name.c_str(),
		ability);

	return info;
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

const shared_ptr<Ability> Player::GetAbility() const
{
	return _ability;
}

const string& Player::GetName() const
{
	return _baseInfo->Name;
}

const int32& Player::GetLevel() const
{
	return _baseInfo->Level;
}

const uint64& Player::GetCharId() const
{
	return _baseInfo->CharId;
}

const uint8& Player::GetCharType() const
{
	return _baseInfo->CharType;
}

void Player::SetAbility(const CharacterAbility* ability)
{
	_ability->STR = ability->STR();
	_ability->DEX = ability->DEX();
	_ability->INT = ability->INT();
	_ability->LUK = ability->LUK();
}

void Player::SetName(const string& name)
{
	_baseInfo->Name = name;
}

void Player::SetLevel(const int32& level)
{
	_baseInfo->Level = level;
}

void Player::SetCharId(const uint64& charId)
{
	_baseInfo->CharId = charId;
}

void Player::SetCharType(const uint8& charType)
{
	_baseInfo->CharType = charType;
}
