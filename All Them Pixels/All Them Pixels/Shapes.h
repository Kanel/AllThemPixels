#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

static class Shapes
{
public:
	enum HexagonStyles
	{
		FlatTopped,
		PointyTopped
	};

public:
	static void hexagon(Vertex vertices[], int start, Vector2f position, float radius, bool flatTopped);
	static void hexagon(Vertex vertices[], int start, Vector2f position, float radius, Color color, bool flatTopped);
	static void rectangle(Vertex vertices[], int start, Vector2f position, float height, float width);
	static void rectangle(Vertex vertices[], int start, Vector2f position, float height, float width, Color color);
};