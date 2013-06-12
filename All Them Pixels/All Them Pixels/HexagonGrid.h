#pragma once

#define _USE_MATH_DEFINES

#include "Hexagon.h"
#include <math.h>


class HexagonGrid
{
protected:
	int tiles;
	Hexagon * shapes;

protected:
	int getNumberOfTiles(int layers);
	int getNumberOfTilesInLayer(int layer);

public:
	HexagonGrid(sf::Vector2f position, int layers, int tileSize, int tileSpacing);
	~HexagonGrid();

	void draw(sf::RenderWindow * window);
};