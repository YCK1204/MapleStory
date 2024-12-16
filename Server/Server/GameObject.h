#pragma once

enum class ObjectType : uint8 {
	PLAYER,
	MONSTER,
	NPC,
	ITEM,
};

class GameObject : public enable_shared_from_this<GameObject>
{
protected:
	ObjectType _type;
	uint64 _id;
public:
	GameObject();
	virtual ~GameObject();
public:
	const uint64& GetId() const;
	const ObjectType& GetType() const;
	void SetId(uint64& id);
	shared_ptr<GameObject> GetPtr();
};