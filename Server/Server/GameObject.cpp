#include "pch.h"
#include "GameObject.h"

GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

shared_ptr<GameObject> GameObject::GetPtr()
{
    return this->shared_from_this();
}
