#pragma once

#include <SFML\Graphics.hpp>

using namespace sf;

static class Vector2fMath
{
public:
	static float dot(Vector2f a, Vector2f b);
	static float cross(Vector2f a, Vector2f b);
	static Vector2f leftNormal(Vector2f vector);
	static Vector2f rightNormal(Vector2f vector);
	static Vector2f invert(Vector2f vector);
	static float distance(Vector2f a, Vector2f b);
	static float length(Vector2f vector);
	static Vector2f unitVector(Vector2f vector);
	static Vector2f max();
	static Vector2f min();
};