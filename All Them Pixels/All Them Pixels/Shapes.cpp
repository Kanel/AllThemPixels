#include "Shapes.h"

void Shapes::hexagon(Vertex vertecies[], int start, Vector2f position, float radius)
{
	hexagon(vertecies, start, position, radius, Color(255, 255, 255));
}

void Shapes::hexagon(Vertex vertecies[], int start, Vector2f position, float radius, Color color)
{
	for (int i = 0; i < 7; i++)
	{
		float x = position.x + cosf(1.0472 * i) * radius;
		float y = position.y + sinf(1.0472 * i) * radius;

		vertecies[start + i] = Vertex(Vector2f(x, y), color);
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