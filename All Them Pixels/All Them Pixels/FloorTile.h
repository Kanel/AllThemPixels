#pragma once

#include "MonotoneChain.h"
#include "UpdateInfo.h"
#include "Hexagon.h"
#include "ColorFade.h"
#include "HexagonGrid.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class FloorTile
{
private:
	Color originalColor;
	Color currentColor;
	Hexagon hexagon;

public:
	FloorTile(Vector2f position, float radius, Color color, VertexCollection * vertexSource);

	Color getColor();
	void setColor(Color color);
	void resetColor();

	void fadeToOriginalColor(int amount);

	Rect<float> getBoundingBox();
	ConvexHull getConvexHull();

	void update(UpdateInfo info);

	static FloorTile *** generateGrid(Vector2f position, float hexagonRadius, int layers, VertexCollection * vertexSource);
};