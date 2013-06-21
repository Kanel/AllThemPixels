#pragma once

#define _USE_MATH_DEFINES

#include "Entity.h"
#include "Shapes.h"

struct SimpleHexagon
{
	Vertex corners[6];
};

class FloorTiles
{
protected:
	Vector2i windowSize;
	SimpleHexagon tiles[100];

protected:

public:
	FloorTiles();
	FloorTiles(Vector2i windowSize, Color colors[3], int tileSize = 100);
	~FloorTiles();

	void move(Vector2f movement);
	void draw(sf::RenderWindow * window);
};