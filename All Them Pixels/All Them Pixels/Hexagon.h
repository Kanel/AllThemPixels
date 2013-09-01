#pragma once

#include "Collision.h"
#include "SAT.h"
#include "VertexCollection.h"
#include <SFML/Graphics.hpp>
#include <stack>

using std::stack;
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
	//int vertexCount;
	int vertexOffset;

private:
	float radius;
	Style style;
	//Vector2f position;
	//Vertex corners[7];
	//Rect<float> boundingBox;
	//ConvexHull convexHull;
	VertexCollection * vertexSource;
	//stack<Color> colorStack;

private:
	void updateColor();

public:	
	Hexagon(Vector2f position, float radius, Color color, Style style, VertexCollection * vertexSource);
	~Hexagon();

	Style getStyle();
	Rect<float> getBoundingBox();
	ConvexHull getConvexHull();
	Vector2f * getPoints(int &count);	

	Color getColor();
	void setColor(Color color);

	void morph(float length, float width);

	void applyTransform(Transform &transform);
};