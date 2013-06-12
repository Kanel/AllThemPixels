#include "Hexagon.h"

Hexagon::Hexagon(sf::Vector2f position)
{
	shape.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
	shape.append(sf::Vertex(sf::Vector2f(position.x, position.y)));

	for (int i = 0; i <= 6; i++)
	{
		shape.append(sf::Vertex(sf::Vector2f(position.x + cosf(1.0472 * i) * 100, position.y + sinf(1.0472 * i) * 100)));
	}
}

sf::VertexArray Hexagon::getShape()
{
	return shape;
}