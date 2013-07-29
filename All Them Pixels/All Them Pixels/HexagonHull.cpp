#include "HexagonHull.h"

HexagonHull::HexagonHull(Vector2f position, int hexagonCount, float hexagonRadius, Hexagon::Style style, VertexCollection * vertexSource)
{
	HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);

	this->hexagonCount = hexagonCount;
	this->hexagons = new Hexagon*[hexagonCount];
	
	for (int i = 0; i < hexagonCount; i++)
	{
		Vector2f partPosition;

		grid.next(partPosition);

		hexagons[i] = new Hexagon(position + partPosition, hexagonRadius - 0.5f, Color::Blue, style, vertexSource);
	}
}

HexagonHull::~HexagonHull()
{
	for (int i = 0; i < hexagonCount; i++)
	{
		delete hexagons[i];
	}
	delete[] hexagons;
}

Vector2f HexagonHull::getPosition()
{
	return position;
}

void HexagonHull::setPosition(Vector2f position)
{
	Transform transform;
	Vector2f movement = position - this->position;

	this->position = position;

	transform.translate(movement);

	applyTransform(transform);
}

void HexagonHull::applyTransform(Transform transform)
{
	for (int i = 0; i < hexagonCount; i++)
	{
		hexagons[i]->applyTransform(transform);
	}
}