#pragma once

#include "Collision.h"
#include "SAT.h"
#include "VertexCollection.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class Hexagon
{
public:
	enum Style
	{
		FlatTopped,
		PointyTopped
	};

public:// should not be public
	int vertexCount;
	int vertexOffset;

private:
	Style style;
	//Vertex corners[7];
	Rect<float> boundingBox;
	ConvexHull convexHull;
	VertexCollection * vertexSource;
	Color originalColor;

public:	
	Hexagon(Vector2f position, float radius, Color color, Style style, VertexCollection * vertexSource);
	~Hexagon();

	Style getStyle();
	Rect<float> getBoundingBox();
	ConvexHull getConvexHull();
	Color getColor();
	void setColor(Color color);

	void applyTransform(Transform transform);
};