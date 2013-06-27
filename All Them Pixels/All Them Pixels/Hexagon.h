#pragma once

#include "SAT.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class Hexagon
{
public:
	enum Style
	{
		FlatTopped,
		PointyTopped
	};

private:
	Style style;
	Vertex corners[6];
	Rect<float> boundingBox;
	ConvexHull convexHull;

public:
	Hexagon();
	Hexagon(Vector2f position, float radius, Color color, Style style);

	Style getStyle();
	Rect<float> getBoundingBox();
	ConvexHull getConvexHull();
	void setColor(Color color);

	void draw(RenderWindow * window);
};