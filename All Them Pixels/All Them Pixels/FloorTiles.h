#pragma once

#define _USE_MATH_DEFINES

#include "Entity.h"
#include "Shapes.h"
#include "HexagonGrid.h"
#include <vector>

using std::vector;

struct SimpleHexagon
{
	Vertex corners[6];
};

class FloorTiles
{
protected:
	Vector2i windowSize;
	vector<SimpleHexagon> tiles;
	int tileCount;

public:
	FloorTiles();
	FloorTiles(Vector2i windowSize, Vector2f position, int layers, int tileSize = 100);

	void draw(sf::RenderWindow * window);
};