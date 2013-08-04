#include "Map.h"

Map::Map() : storage(0)
{
	layers = 0;
}

Map::Map(Vector2f position, int layers, float hexagonRadius, Hexagon::Style style) : storage(0)
{
	HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);

	this->mapVertexCollection = new VertexCollection(8, grid.getNumberOfTiles(layers), 1, PrimitiveType::TrianglesStrip);
	this->layers = layers;
	this->storage = grid.generateGrid(position, layers, mapVertexCollection);	
}

Map::~Map()
{
	int tiles = HexagonGrid::getNumberOfTiles(layers);
	HexagonGrid grid(Hexagon::FlatTopped);

	for (int i = 0; i < tiles; i++)
	{
		AxialCoordinates coordinates = grid.next();

		delete storage[coordinates.q][coordinates.r];
	}
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
		storage[beenThere[i]]->setColor(Color::Blue);
	}

	for (i = 0; i < doneThat.size(); i++)
	{
		storage[doneThat[i]]->setColor(Color::Green);
	}

	storage[playerLocation]->setColor(Color::Black);
}

void Map::applyTransform(Transform transform)
{
	int tiles = HexagonGrid::getNumberOfTiles(layers);
	HexagonGrid grid(Hexagon::FlatTopped);

	for (int i = 0; i < tiles; i++)
	{
		AxialCoordinates coordinates = grid.next();

		storage[coordinates.q][coordinates.r]->applyTransform(transform);
	}
}

void Map::draw(RenderTarget& target, RenderStates states) const
{	
	target.draw(*mapVertexCollection);
}