#pragma once

#include <SFML\Graphics.hpp>
#include <math.h>

using namespace sf;

class Hexagon
{
protected:
	int radius;
	Vector2f position;
	VertexArray shape;

public:
	Hexagon();
	Hexagon(Vector2f position, int radius);

	Rect<float> getBoundingBox();
	void setColor(Color color);

	VertexArray getShape();
};