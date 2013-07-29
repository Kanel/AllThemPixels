#pragma once

#include "Hexagon.h"
#include "HexagonGrid.h"

class HexagonHull
{
private:
	int hexagonCount;
	Vector2f position;
	Hexagon ** hexagons;

public:
	HexagonHull(Vector2f position, int hexagonCount, float hexagonRadius, Hexagon::Style style, VertexCollection * vertexSource);
	~HexagonHull();

	Vector2f getPosition();
	void setPosition(Vector2f position);

	void applyTransform(Transform transform);

	Rect<float> getBoundingBox();
	ConvexHull getConvexHull();
};