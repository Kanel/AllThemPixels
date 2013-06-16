#include "Hexagon.h"

Hexagon::Hexagon() { }

Hexagon::Hexagon(Vector2f position, int radius)
{
	this->position = position;
	this->radius = radius;

	shape.setPrimitiveType(PrimitiveType::TrianglesFan);
	shape.append(Vertex(Vector2f(position.x, position.y)));

	for (int i = 0; i <= 6; i++)
	{
		shape.append(Vertex(Vector2f(position.x + cosf(1.0472 * i) * radius, position.y + sinf(1.0472 * i) * radius)));
	}
}

Rect<float> Hexagon::getBoundingBox()
{
	return Rect<float>(position.x - radius, position.y - radius, radius * 2, radius * 2);
}

void Hexagon::setColor(Color color)
{
	for (int i = 0; i < 8; i++)
	{
		shape[i].color = color;
	}
}

VertexArray Hexagon::getShape()
{
	return shape;
}