#pragma once

#include <SFML\Graphics.hpp>

using namespace sf;

static class Shapes
{
public:
	static void hexagon(Vertex vertecies[], int start, Vector2f position, float radius);
	static void hexagon(Vertex vertecies[], int start, Vector2f position, float radius, Color color);
	static void rectangle(Vertex vertecies[], int start, Vector2f position, float height, float width);
	static void rectangle(Vertex vertecies[], int start, Vector2f position, float height, float width, Color color);

	// todo: remove?
	// http://wiki.processing.org/w/Rect-Rect_intersection
	static bool contains(Rect<float> a, Rect<float> b);
};