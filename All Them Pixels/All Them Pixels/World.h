#pragma once

#include "Territory.h"
#include "HexagonGrid.h"
#include "VertexCluster.h"
#include "Sounds.h"
#include <SFML/Graphics.hpp>
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
	Player * player;
	Vector2i offset;
	AxialCoordinates currentTerritoryCoordinates;
	Territory *** territories;
	VertexCluster playerCluster;

private:
	vector<AIProperties> getAIProperties();
	vector<WeaponConfiguration> getWeaponConfigurations();

public:
	World(Vector2f position, float territoryRadius, float territorySpacing, int layers);

	Territory * getCurrentTerritory();
	Territory * getTerritory(AxialCoordinates coordinates);
	void changeTerritory(Vector2f position);

	void activate(AxialCoordinates coordinates);
	bool isActive();
	bool isCleared();

	void draw(RenderWindow * window);
	void update(UpdateInfo info, Sounds * sounds);
	View getView(View reference);
};