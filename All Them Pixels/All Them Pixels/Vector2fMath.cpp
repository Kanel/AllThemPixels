#include "Vector2fMath.h"

float Vector2fMath::dot(Vector2f a, Vector2f b)
{
	return (a.x * b.x) + (a.y * b.y);
}

float Vector2fMath::cross(Vector2f a, Vector2f b)
{
	return (a.x * b.y) - (a.y * b.x);
}

Vector2f Vector2fMath::leftNormal(Vector2f vector)
{
	return Vector2f(-vector.y, vector.x);
}

Vector2f Vector2fMath::rightNormal(Vector2f vector)
{
	return Vector2f(vector.y, -vector.x);
}

Vector2f Vector2fMath::invert(Vector2f vector)
{
	return Vector2f(-vector.x, -vector.y);
}

float Vector2fMath::distance(Vector2f a, Vector2f b)
{
	return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
}

float Vector2fMath::length(Vector2f vector)
{
	return sqrtf(powf(vector.x, 2) + powf(vector.y, 2));
}

Vector2f Vector2fMath::unitVector(Vector2f vector)
{
	float l= length(vector);

	if (l != 0)
	{
		return Vector2f(vector.x / l, vector.y / l);
	}
	else
	{
		return Vector2f();
	}
}

Vector2f Vector2fMath::max()
{
	return Vector2f(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
}

Vector2f Vector2fMath::min()
{
	return Vector2f(std::numeric_limits<float>::min(), std::numeric_limits<float>::min());
}

Vector2f Vector2fMath::turn90left(Vector2f vector)
{
	return Vector2f(vector.y, -vector.x);
}

Vector2f Vector2fMath::turn90right(Vector2f vector)
{
	return Vector2f(-vector.y, vector.x);
}