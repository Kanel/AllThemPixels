#include "Hexagon.h"

Hexagon::Hexagon() { }

Hexagon::Hexagon(sf::Vector2f position, int radius)
{
	shape.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
	shape.append(sf::Vertex(sf::Vector2f(position.x, position.y)));

	for (int i = 0; i <= 6; i++)
	{
		shape.append(sf::Vertex(sf::Vector2f(position.x + cosf(1.0472 * i) * radius, position.y + sinf(1.0472 * i) * radius)));
	}
}

sf::VertexArray Hexagon::getShape()
{
	return shape;
}