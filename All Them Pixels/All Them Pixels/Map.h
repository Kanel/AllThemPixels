#pragma once

#include "Hexagon.h"
#include "HexagonGrid.h"
#include "VertexCollection.h"

class Map : public Drawable
{
private:
	int hexagonLength;
	Vector2f position;
	Hexagon *** hexagons;
	VertexCollection * mapVertexCollection;
	vector<AxialCoordinates> beenThere;
	vector<AxialCoordinates> doneThat;
	AxialCoordinates playerLocation;
	Vector2i offset;

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
	void applyTransform(Transform transform);

};