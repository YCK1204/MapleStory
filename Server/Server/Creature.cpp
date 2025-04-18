#include "pch.h"
#include "Creature.h"

Vector2::Vector2() { X = 0; Y = 0; }
Vector2::Vector2(float x, float y) { X = x; Y = y; }
Vector2::Vector2(const Vector2& vec) { *this = vec; }
void Vector2::Copy(const Vector2& vec) { *this = vec; }
Vector2 Vector2::operator + (const Vector2& vec) const { return Vector2(X + vec.X, Y + vec.Y); }
Vector2 Vector2::operator - (const Vector2& vec) const { return Vector2(X - vec.X, Y - vec.Y); }
Vector2& Vector2::operator = (const Vector2& vec) { X = vec.X; Y = vec.Y; return *this; }
Vector2 Vector2::operator * (const Vector2& vec) const { return Vector2(X * vec.X, Y * vec.Y); }
Vector2 Vector2::operator + (float f) const { return Vector2(X + f, Y + f); }
Vector2 Vector2::operator - (float f) const { return Vector2(X - f, Y - f); }
Vector2 Vector2::operator = (float f) const { return Vector2(f, f); }
Vector2 Vector2::operator * (float f) const { return Vector2(X * f, Y * f); }
Vector2& Vector2::operator += (const Vector2& vec) { this->X += vec.X; this->Y += vec.Y; return *this; }
Vector2& Vector2::operator -= (const Vector2& vec) { this->X += vec.X; this->Y += vec.Y; return *this;
}
Vector2& Vector2::operator *= (const Vector2& vec) { this->X += vec.X; this->Y += vec.Y; return *this; }
Vector2& Vector2::operator += (float f) { this->X += f; this->Y += f; return *this; }
Vector2& Vector2::operator -= (float f) { this->X += f; this->Y += f; return *this; }
Vector2& Vector2::operator *= (float f) { this->X += f; this->Y += f; return *this; }
const Vector2 Vector2::zero(0, 0);
const Vector2 Vector2::left(-1, 0);
const Vector2 Vector2::right(1, 0);
const Vector2 Vector2::up(0, 1);
const Vector2 Vector2::down(0, -1);

Creature::Creature()
{
	Pos->X = 0;
	Pos->Y = 0;
}

Creature::~Creature()
{
}


Offset<Position> Creature::GeneratePosition(FlatBufferBuilder& builder)
{
	return CreatePosition(builder, Pos->X, Pos->Y);
}