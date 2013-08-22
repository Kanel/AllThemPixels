#pragma once

#include "Territory.h"
#include "HexagonGrid.h"
#include "Map.h"
#include "VertexCluster.h"
#include "Sounds.h"
#include "HexagonGridStorage.h"
#include <SFML/Graphics.hpp>
#include <vector>

using std::vector;
using namespace sf;

class Territory;

class World : public Drawable
{
private:
	float territoryRadius;
	float territorySpacing;
	int layers;
	Player * player;
	AxialCoordinates currentTerritoryCoordinates;
	HexagonGridStorage<Territory *> territories;
	Map * map;

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

	Player * getPlayer() const;

	virtual void draw(RenderTarget& target, RenderStates states) const;

	void update(UpdateInfo info, Controls * controls, Sounds * sounds);
	View getView(View reference);
};