#pragma once

#include "Config.h"
#include "Hexagon.h"
#include "HexagonGrid.h"
#include "VertexCollection.h"

class Map : public Drawable
{
private:
	int layers;
	Vector2f position;
	HexagonGridStorage<Hexagon *> storage;
	VertexCollection * mapVertexCollection;
	vector<AxialCoordinates> beenThere;
	vector<AxialCoordinates> doneThat;
	AxialCoordinates playerLocation;

public:
	Map();
	Map(Vector2f position, int layers, float hexagonRadius, Hexagon::Style style);
	~Map();

	void addClearedTerritory(AxialCoordinates what);
	void setPlayerLocation(AxialCoordinates where);

	void setPosition(Vector2f position);
	void colorize();

	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	void applyTransform(Transform &transform);

};