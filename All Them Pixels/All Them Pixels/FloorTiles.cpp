#include "FloorTiles.h"
#include "Shapes.h"

FloorTiles::FloorTiles()
{

}

FloorTiles::FloorTiles(Vector2i windowSize, Color colors[3], int tileSize)
{
	this->windowSize = windowSize;
	int line, column, offset;
	float width = sqrt(3 * (tileSize * tileSize) / 4);
	for (int i = 0; i < 100; i++)
	{
		line = int(i / 10);
		column = i % 10;
		offset = (line % 2) ? width : 0;
		Shapes::hexagon(tiles[i].corners, 0, Vector2f(2 * column * width + offset, line * (tileSize + tileSize / 2)), tileSize, colors[(2 * (line % 2) + column % 2)]);
	}
}

FloorTiles::~FloorTiles()
{

}

void FloorTiles::move(Vector2f movement)
{
	float width = sqrt(3 * (200 * 200) / 4);

	float x = tiles[0].corners[0].position.x;
	float y = tiles[0].corners[0].position.y;

	if (x + movement.x < -200)
		movement.x += 2 * width;
	if (x + movement.x > 0)
		movement.x -= 2 * width;
	if (y + movement.y < -300)
		movement.y += 300;
	if (y + movement.y > -100)
		movement.y -= 300;

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			tiles[i].corners[j].position.x += movement.x;
			tiles[i].corners[j].position.y += movement.y;
		}
	}

}

void FloorTiles::draw(sf::RenderWindow * window)
{
	for (int i = 0; i < 100; i++)
	{
		window->draw(tiles[i].corners, 6, PrimitiveType::TrianglesFan);
	}
}