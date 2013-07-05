#pragma once

#include "Territory.h"
#include "HexagonGrid.h"
#include "VertexCluster.h"
#include <SFML\Graphics.hpp>
#include <vector>

using std::vector;
using namespace sf;

class Territory;

class World
{
private:
	float territoryRadius;
	float territorySpacing;
	int layers;
	int tileSize;
	int territoryCount;
	int matrixSize;
	Vector2i offset;
	AxialCoordinates currentTerritoryCoordinates;
	Territory *** territories;

public:
	World(Vector2f position, float territoryRadius, float territorySpacing, int layers, VertexCluster * cluster);

	Territory * getTerritory(AxialCoordinates coordinates);
	void changeTerritory(Vector2f position);

	bool isActive();

	void draw(RenderWindow * window);
	void update(UpdateInfo info);
	View getView(View reference);
};