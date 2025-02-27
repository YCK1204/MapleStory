#include "pch.h"
#include "Player.h"

Player::Player(ClientRef session)
{
	Session = session;
	Type = ObjectType::PLAYER;
	_baseInfo->MapId = -1;
}

Player::~Player()
{
}

void Player::TakeDamage(int32& damage)
{
	_baseInfo->Hp -= damage;
	if (_baseInfo->Hp <= 0)
		_baseInfo->Hp = 0;
	// todo Destory
}

Offset<PlayerInfo> Player::GeneratePlayerInfo(FlatBufferBuilder& builder)
{
	auto info = GeneratePreviewInfo(builder);
	auto position = CreatePosition(builder, Pos->X, Pos->Y);
	auto playerInfo = CreatePlayerInfo(builder, info, position);

	return playerInfo;
}

Offset<CharacterTotalInfo> Player::GenerateTotalInfo(FlatBufferBuilder& builder)
{
	auto info = GeneratePreviewInfo(builder);

	auto totalInfo = CreateCharacterTotalInfo(
		builder,
		info,
		_baseInfo->MapId,
		_baseInfo->Hp,
		_baseInfo->Mp,
		_baseInfo->Exp);

	return totalInfo;
}

Offset<CharacterPreviewInfo> Player::GeneratePreviewInfo(FlatBufferBuilder& builder)
{
	auto ability = CreateCharacterAbility(
		builder,
		_ability->STR,
		_ability->DEX,
		_ability->INT,
		_ability->LUK);

	auto info = CreateCharacterPreviewInfoDirect(
		builder,
		Id,
		_baseInfo->CharType,
		_baseInfo->Level,
		_baseInfo->Name.c_str(),
		ability);

	return info;
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

const int32& Player::GetHp() const
{
	return _baseInfo->Hp;
}

const int32& Player::GetMp() const
{
	return _baseInfo->Mp;
}

const uint8& Player::GetMapId() const
{
	return _baseInfo->MapId;
}

const int32& Player::GetExp() const
{
	return _baseInfo->Exp;
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

void Player::SetHp(const int32& hp)
{
	_baseInfo->Hp = hp;
}

void Player::SetMp(const int32& mp)
{
	_baseInfo->Mp = mp;
}

void Player::SetMapId(const uint8& mapId)
{
	_baseInfo->MapId = mapId;
}

void Player::SetExp(const int32& exp)
{
	_baseInfo->Exp = exp;
}

const bool Player::IsAlive() const
{
	return (_baseInfo->Hp < 0);
}
