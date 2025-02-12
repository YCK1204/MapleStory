#pragma once

#include "Creature.h"
#include "GameRoom.h"

typedef struct {
	int32 STR;
	int32 DEX;
	int32 INT;
	int32 LUK;
} Ability;

typedef struct {
	int32 Level;
	uint64 CharId;
	uint8 CharType;
	string Name;
} BasePlayerInfo;

class Player : public Creature
{
private:
	shared_ptr<Ability> _ability = make_shared<Ability>();
	shared_ptr<BasePlayerInfo> _baseInfo = make_shared<BasePlayerInfo>();
public:
	weak_ptr<ClientSession> Session;
	GameRoomRef Room = nullptr;

public:
	uint8 CurMapId = -1;
public:
	Player(ClientRef session);
	virtual ~Player();
	virtual void TakeDamage(int32& damage);
	Offset<CharacterPreviewInfo> GenerateCharPreviewInfo(FlatBufferBuilder& builder);
public:
	void EnterRoom(uint8& roomId);
	void LeaveRoom(uint8& roomId);
public:
	const shared_ptr<Ability> GetAbility() const;
	const string& GetName() const;
	const int32& GetLevel() const;
	const uint64& GetCharId() const;
	const uint8& GetCharType() const;
public:
	void SetAbility(const struct CharacterAbility* ability);
	void SetName(const string& name);
	void SetLevel(const int32& level);
	void SetCharId(const uint64& charId);
	void SetCharType(const uint8& charType);
};