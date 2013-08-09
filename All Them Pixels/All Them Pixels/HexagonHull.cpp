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

		hexagons[i] = new Hexagon(position + partPosition, hexagonRadius - 0.5f, Color(75, 125, 255), style, vertexSource);
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

Rect<float> HexagonHull::getBoundingBox()
{
	float minX = FLT_MAX;
	float maxX = FLT_MIN;
	float minY = FLT_MAX;
	float maxY = FLT_MIN;

	for (int i = 0; i < hexagonCount; i++)
	{
		int count;
		Vector2f * points = hexagons[i]->getPoints(count);

		for (int j = 0; j < count; j++)
		{
			if (points[j].x < minX)
			{
				minX = points[j].x;
			}
			else if (maxX < points[j].x)
			{
				maxX = points[j].x;
			}

			if (points[j].y < minY)
			{
				minY = points[j].y;
			}
			else if (maxY < points[j].y)
			{
				maxY = points[j].y;
			}
		}
		delete[] points;
	}
	return Rect<float>(minX, minY, maxX - minX, maxY - minY);
}

ConvexHull HexagonHull::getConvexHull()
{
	vector<Vector2f> vertices;

	for (int i = 0; i < hexagonCount; i++)
	{
		int count;
		Vector2f * points = hexagons[i]->getPoints(count);

		for (int j = 1; j < count; j++)
		{
			vertices.push_back(points[i]);
		}
		delete[] points;
	}
	return MonotoneChain::getConvexHull(vertices);
}