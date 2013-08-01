#include "Map.h"

Map::Map(){}

Map::Map(Vector2f position, int layers, float hexagonRadius, Hexagon::Style style)
{
	HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);

	mapVertexCollection = VertexCollection(8, grid.getNumberOfTiles(layers), 10, PrimitiveType::TrianglesStrip);
	hexagonCount = layers; //grid.getNumberOfTiles(layers);
	hexagons = grid.generateGrid(position, layers, &mapVertexCollection);
	
}

Map::~Map()
{
	for (int i = 0; i < hexagonCount; i++)
	{
		delete hexagons[i];
	}
	delete[] hexagons;
}

void Map::addClearedTerritory(AxialCoordinates what)
{
	hexagons[what.q][what.r]->setColor(Color::Green);
	doneThat.push_back(what);
	colorize();
}

void Map::setPlayerLocation(AxialCoordinates where)
{
	beenThere.push_back(where);
	playerLocation = where;
	colorize();
}

void Map::setPosition(Vector2f position)
{
	Transform transform;
	Vector2f movement = position - this->position;

	this->position = position;

	transform.translate(movement);

	applyTransform(transform);
}

void Map::colorize()
{
	int i;
	for (i = 0; i < beenThere.size(); i++)
	{
		hexagons[beenThere[i].q][beenThere[i].r]->setColor(Color::Blue);
	}
	for (i = 0; i < doneThat.size(); i++)
	{
		hexagons[beenThere[i].q][beenThere[i].r]->setColor(Color::Green);
	}
	hexagons[playerLocation.q][playerLocation.r]->setColor(Color::Black);
}

void Map::applyTransform(Transform transform)
{
	for (int i = 0; i < hexagonCount; i++)
	{
		for (int j = 0; j < hexagonCount; j++)
			hexagons[i][j]->applyTransform(transform);
	}
}

void Map::draw(RenderTarget& target, RenderStates states) const
{	
	target.draw(mapVertexCollection);
}