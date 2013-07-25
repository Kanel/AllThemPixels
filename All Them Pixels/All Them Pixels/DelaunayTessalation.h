#pragma once

#include "Collision.h"
#include <SFML\Graphics.hpp>

using namespace sf;

// Bowyer–Watson algorithm
static class DelaunayTessalation
{
private:
	static bool containsPoint(Vector2f A, float a_x_2, float a_y_2, Vector2f B, float b_x_2, float b_y_2, Vector2f C, float c_x_2, float c_y_2, Vector2f point, float point_x_2, float point_y_2);
	static bool interssects(Vector2f a, Vector2f b, Vector2f c, Vector2f d);
	static bool isLeftOfLine(Vector2f a, Vector2f b, Vector2f c);
	static bool sameSide(Vector2f a, Vector2f b, Vector2f c, Vector2f point);
	static bool isInTriangle(Vector2f a, Vector2f b, Vector2f c, Vector2f point);

public:
	static Vertex *  getTriangles(Vertex * vertices, int count, int &triangleCount);
};
