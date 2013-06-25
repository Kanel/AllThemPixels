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
	vector<SimpleHexagon> tiles;
	int tileCount;

protected:
	Vector3f convertToCubeCoordinates(Vector2f axialCoordinates);
	Vector2i convertToAxialCoordinates(Vector3i cubeCoordinates);
	Vector3i hexRound(Vector3f cubeCoordiantes);
	Vector2f getHexagonPosition(int q, int r, float size);
	int getNumberOfTiles(int layers);

public:
	FloorTiles();
	FloorTiles(Vector2i windowSize, Vector2f position, int layers, int tileSize = 100);

	void draw(sf::RenderWindow * window);
};