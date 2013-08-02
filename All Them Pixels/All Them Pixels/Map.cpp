#include "Map.h"

Map::Map()
{
	hexagonLength = 1;
}

Map::Map(Vector2f position, int layers, float hexagonRadius, Hexagon::Style style)
{
	HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);

	mapVertexCollection = new VertexCollection(8, grid.getNumberOfTiles(layers), 1, PrimitiveType::TrianglesStrip);
	hexagonLength = (layers * 2) + 1;
	offset = Vector2i(layers, layers);
	hexagons = grid.generateGrid(position, layers, mapVertexCollection);	
}

Map::~Map()
{
	for (int i = 0; i < hexagonLength; i++)
	{
		for (int j = 0; j < hexagonLength; j++)
		{
			if (hexagons[i][j] != NULL)
			{
				delete hexagons[i][j];
			}
		}
		delete[] hexagons[i];
	}
	delete[] hexagons;

	delete mapVertexCollection;
}

void Map::addClearedTerritory(AxialCoordinates what)
{
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
		hexagons[offset.x + beenThere[i].q][offset.y + beenThere[i].r]->setColor(Color::Blue);
	}
	for (i = 0; i < doneThat.size(); i++)
	{
		hexagons[offset.x + doneThat[i].q][offset.y + doneThat[i].r]->setColor(Color::Green);
	}
	hexagons[offset.x + playerLocation.q][offset.y + playerLocation.r]->setColor(Color::Black);
}

void Map::applyTransform(Transform transform)
{
	for (int i = 0; i < hexagonLength; i++)
	{
		for (int j = 0; j < hexagonLength; j++)
		{
			if (hexagons[i][j] != NULL)
			{
				hexagons[i][j]->applyTransform(transform);
			}
		}
	}
}

void Map::draw(RenderTarget& target, RenderStates states) const
{	
	target.draw(*mapVertexCollection);
}