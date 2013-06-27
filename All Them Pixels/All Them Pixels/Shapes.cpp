#include "Shapes.h"

void Shapes::hexagon(Vertex vertecies[], int start, Vector2f position, float radius, bool flatTopped)
{
	hexagon(vertecies, start, position, radius, Color(255, 255, 255), flatTopped);
}

void Shapes::hexagon(Vertex vertecies[], int start, Vector2f position, float radius, Color color, bool flatTopped)
{
	if (flatTopped)
	{
		// Flat topped
		float width = radius * 2;
		float halfWidth = radius;
		float quarterWidth = radius / 2;
		float height = sqrt(3)/2 * width;
		float halfHeight = height / 2;

		vertecies[start + 0] = Vertex(Vector2f(position.x - quarterWidth, position.y - halfHeight), color);
		vertecies[start + 1] = Vertex(Vector2f(position.x + quarterWidth, position.y - halfHeight), color);
		vertecies[start + 2] = Vertex(Vector2f(position.x + halfWidth, position.y), color);
		vertecies[start + 3] = Vertex(Vector2f(position.x + quarterWidth, position.y + halfHeight), color);
		vertecies[start + 4] = Vertex(Vector2f(position.x - quarterWidth, position.y + halfHeight), color);
		vertecies[start + 5] = Vertex(Vector2f(position.x - halfWidth, position.y), color);
	}
	else
	{
		// Pointy topped
		float quarterHeight = radius / 2;
		float halfHeight = radius;
		float height = radius * 2;
		float width = sqrtf(3) / 2 * height;
		float halfWidth = width / 2;

		vertecies[start + 0] = Vertex(Vector2f(position.x, position.y - halfHeight), color);
		vertecies[start + 1] = Vertex(Vector2f(position.x + halfWidth, position.y - quarterHeight), color);
		vertecies[start + 2] = Vertex(Vector2f(position.x + halfWidth, position.y + quarterHeight), color);
		vertecies[start + 3] = Vertex(Vector2f(position.x, position.y + halfHeight), color);
		vertecies[start + 4] = Vertex(Vector2f(position.x - halfWidth, position.y + quarterHeight), color);
		vertecies[start + 5] = Vertex(Vector2f(position.x - halfWidth, position.y - quarterHeight), color);
	}
}

void Shapes::rectangle(Vertex vertecies[], int start, Vector2f position, float height, float width)
{
	rectangle(vertecies, start, position, height, width, Color(255, 255, 255));
}

void Shapes::rectangle(Vertex vertecies[], int start, Vector2f position, float height, float width, Color color)
{
	vertecies[start + 0] = Vertex(Vector2f(position.x - (width / 2), position.y - (height / 2)), color);
	vertecies[start + 1] = Vertex(Vector2f(position.x + (width / 2), position.y - (height / 2)), color);
	vertecies[start + 2] = Vertex(Vector2f(position.x + (width / 2), position.y + (height / 2)), color);
	vertecies[start + 3] = Vertex(Vector2f(position.x - (width / 2), position.y + (height / 2)), color);
}

bool Shapes::contains(Rect<float> a, Rect<float> b)
{
	float x_1 = a.left;
	float y_1 = a.top;
	float width_1 = a.width;
	float height_1 = a.height;
	float x_2 = b.left;
	float y_2 = b.top;
	float width_2 = b.width;
	float height_2 = b.height;

	return !(x_1 > x_2+width_2 || x_1+width_1 < x_2 || y_1 > y_2+height_2 || y_1+height_1 < y_2);
}