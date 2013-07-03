#include "Shapes.h"

void Shapes::hexagon(Vertex vertices[], int start, Vector2f position, float radius, bool flatTopped)
{
	hexagon(vertices, start, position, radius, Color(255, 255, 255), flatTopped);
}

void Shapes::hexagon(Vertex vertices[], int start, Vector2f position, float radius, Color color, bool flatTopped)
{
	if (flatTopped)
	{
		// Flat topped
		float width = radius * 2;
		float halfWidth = radius;
		float quarterWidth = radius / 2;
		float height = sqrt(3)/2 * width;
		float halfHeight = height / 2;

		vertices[start + 0] = Vertex(Vector2f(position.x - quarterWidth, position.y - halfHeight), color);
		vertices[start + 1] = Vertex(Vector2f(position.x + quarterWidth, position.y - halfHeight), color);
		vertices[start + 2] = Vertex(Vector2f(position.x + halfWidth, position.y), color);
		vertices[start + 3] = Vertex(Vector2f(position.x + quarterWidth, position.y + halfHeight), color);
		vertices[start + 4] = Vertex(Vector2f(position.x - quarterWidth, position.y + halfHeight), color);
		vertices[start + 5] = Vertex(Vector2f(position.x - halfWidth, position.y), color);
	}
	else
	{
		// Pointy topped
		float quarterHeight = radius / 2;
		float halfHeight = radius;
		float height = radius * 2;
		float width = sqrtf(3) / 2 * height;
		float halfWidth = width / 2;

		vertices[start + 0] = Vertex(Vector2f(position.x, position.y - halfHeight), color);
		vertices[start + 1] = Vertex(Vector2f(position.x + halfWidth, position.y - quarterHeight), color);
		vertices[start + 2] = Vertex(Vector2f(position.x + halfWidth, position.y + quarterHeight), color);
		vertices[start + 3] = Vertex(Vector2f(position.x, position.y + halfHeight), color);
		vertices[start + 4] = Vertex(Vector2f(position.x - halfWidth, position.y + quarterHeight), color);
		vertices[start + 5] = Vertex(Vector2f(position.x - halfWidth, position.y - quarterHeight), color);
	}
}

void Shapes::rectangle(Vertex vertices[], int start, Vector2f position, float height, float width)
{
	rectangle(vertices, start, position, height, width, Color(255, 255, 255));
}

void Shapes::rectangle(Vertex vertices[], int start, Vector2f position, float height, float width, Color color)
{
	vertices[start + 0] = Vertex(Vector2f(position.x - (width / 2), position.y - (height / 2)), color);
	vertices[start + 1] = Vertex(Vector2f(position.x + (width / 2), position.y - (height / 2)), color);
	vertices[start + 2] = Vertex(Vector2f(position.x + (width / 2), position.y + (height / 2)), color);
	vertices[start + 3] = Vertex(Vector2f(position.x - (width / 2), position.y + (height / 2)), color);
}