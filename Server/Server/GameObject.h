#pragma once

enum class ObjectType : uint8 {
	PLAYER,
	MONSTER,
	NPC,
	ITEM,
};

class GameObject : public enable_shared_from_this<GameObject>
{
public:
	ObjectType Type;
	uint64 Id;
public:
	GameObject();
	virtual ~GameObject();
public:
	shared_ptr<GameObject> GetPtr();
};