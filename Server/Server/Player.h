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
	int32 Hp;
	int32 Mp;
	int32 Exp;
	uint8 MapId;
} BasePlayerInfo;

enum class PlayerState : uint8
{	
	NONE,
	STAND,
	MOVE,
	JUMP,
	PRONE_STAB,
	PRONE_ATTACK,
	ATTACK,
};

class Player : public Creature
{
private:
	shared_ptr<Ability> _ability = make_shared<Ability>();
	shared_ptr<BasePlayerInfo> _baseInfo = make_shared<BasePlayerInfo>();
	uint32 State;

public:
	weak_ptr<ClientSession> Session;
	GameRoomRef Room = nullptr;

public:
	Player();
	virtual ~Player();
	virtual void TakeDamage(int32& damage);
	const bool IsInState(PlayerState state);
	Offset<PlayerInfo> GeneratePlayerInfo(FlatBufferBuilder& builder);
	Offset<CharacterTotalInfo> GenerateTotalInfo(FlatBufferBuilder& builder);
	Offset<CharacterPreviewInfo> GeneratePreviewInfo(FlatBufferBuilder& builder);
public:
	const shared_ptr<Ability> GetAbility() const;
	const string& GetName() const;
	const int32& GetLevel() const;
	const uint64& GetCharId() const;
	const uint8& GetCharType() const;
	const int32& GetHp() const;
	const int32& GetMp() const;
	const uint8& GetMapId() const;
	const int32& GetExp() const;

public:
	void SetAbility(const struct CharacterAbility* ability);
	void SetName(const string& name);
	void SetLevel(const int32& level);
	void SetCharId(const uint64& charId);
	void SetCharType(const uint8& charType);
	void SetHp(const int32& hp);
	void SetMp(const int32& mp);
	void SetMapId(const uint8& mapId);
	void SetExp(const int32& exp);
	void RemoveState(const PlayerState state);
	void AddState(const PlayerState state);
	void ClearState();
protected:
	virtual const bool IsAlive() const;
};