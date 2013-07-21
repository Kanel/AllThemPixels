#pragma once

#include "MonotoneChain.h"
#include "UpdateInfo.h"
#include "Hexagon.h"
#include "ColorFade.h"
#include "HexagonGrid.h"
#include <stack>
#include <SFML\Graphics.hpp>

using std::stack;
using namespace sf;

class FloorTile
{
private:
	stack<Color> colorStack;
	Hexagon hexagon;

public:
	FloorTile(Vector2f position, float radius, Color color, VertexCollection * vertexSource);

	Color getColor();
	void setColor(Color color);
	void pushColor(Color color);
	void resetColor();
	
	Rect<float> getBoundingBox();
	ConvexHull getConvexHull();

	void update(UpdateInfo info);

	static FloorTile *** generateGrid(Vector2f position, float hexagonRadius, int layers, VertexCollection * vertexSource);
};