#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

const uint64& GameObject::GetId() const
{
    return _id;
}

const ObjectType& GameObject::GetType() const
{
    return _type;
}

void GameObject::SetId(uint64& id)
{
    _id = id;
}

shared_ptr<GameObject> GameObject::GetPtr()
{
    return this->shared_from_this();
}
