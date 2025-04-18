#pragma once

#include "GameObject.h"

class Vector2 {
public:
	float X;
	float Y;
	Vector2();
	Vector2(float x, float y);
	Vector2(const Vector2& vec);
	void Copy(const Vector2& vec);
	Vector2 operator + (const Vector2& vec) const;
	Vector2 operator - (const Vector2& vec) const;
	Vector2& operator = (const Vector2& vec);
	Vector2 operator * (const Vector2& vec) const;
	Vector2 operator + (float f) const;
	Vector2 operator - (float f) const;
	Vector2 operator = (float f) const;
	Vector2 operator * (float f) const;
	Vector2& operator += (const Vector2& vec);
	Vector2& operator -= (const Vector2& vec);
	Vector2& operator *= (const Vector2& vec);
	Vector2& operator += (float f);
	Vector2& operator -= (float f);
	Vector2& operator *= (float f);
	const static Vector2 zero;
	const static Vector2 left;
	const static Vector2 right;
	const static Vector2 up;
	const static Vector2 down;
};

class Creature : public GameObject
{
public:
	shared_ptr<Vector2> Pos = make_shared<Vector2>();
public:
	Creature();
	virtual ~Creature();

public:
	Offset<Position> GeneratePosition(FlatBufferBuilder& builder);
	virtual const bool IsAlive() const = 0;
	virtual void TakeDamage(int32& damage) = 0;
};