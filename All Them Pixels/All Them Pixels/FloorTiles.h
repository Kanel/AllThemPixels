#pragma once

#define _USE_MATH_DEFINES

#include "Entity.h"
#include "Shapes.h"
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
	//SimpleHexagon tiles[100];
	vector<SimpleHexagon> tiles;
	int tileCount;

protected:

public:
	FloorTiles();
	FloorTiles(Vector2i windowSize, Vector2f position, Color colors[3], int layers, int tileSize = 100);
	~FloorTiles();

	void move(Vector2f movement);
	void draw(sf::RenderWindow * window);
};