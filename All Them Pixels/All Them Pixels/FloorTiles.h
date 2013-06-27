#pragma once

#define _USE_MATH_DEFINES

#include "Entity.h"
#include "Shapes.h"
#include "HexagonGrid.h"
#include <vector>

using std::vector;

// Should this be made into an class? ... currently unsure ...
struct SimpleHexagon
{
	Vertex corners[6];
	Rect<float> boundingBox;
};

class FloorTiles
{
protected:
	Vector2i windowSize;
	vector<SimpleHexagon> tiles;
	vector<SimpleHexagon> border;
	int tileCount;
	int borderCount;

public:
	FloorTiles();
	FloorTiles(Vector2i windowSize, Vector2f position, int layers, int tileSize = 100);

	bool intersectsBorder(Entity * entity);

	void draw(sf::RenderWindow * window);
};