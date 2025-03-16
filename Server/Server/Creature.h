#pragma once

#include "GameObject.h"

typedef struct vector2 {
	float X;
	float Y;
	struct vector2() { X = 0; Y = 0; }
	struct vector2(float x, float y) { X = x; Y = y; }
	struct vector2(struct vector2& vec) { this->X = vec.X; this->Y = vec.Y; }
	struct vector2 operator + (const struct vector2& vec) { this->X += vec.X; this->Y += vec.Y; return *this; }
	struct vector2 operator - (const struct vector2& vec) { this->X -= vec.X; this->Y -= vec.Y; return *this; }
	struct vector2 operator = (const struct vector2& vec) { this->X = vec.X; this->Y = vec.Y; return *this; }
} Vector2;

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