#include "FloorTiles.h"
#include "Shapes.h"

FloorTiles::FloorTiles() { }

FloorTiles::FloorTiles(Vector2i windowSize, Vector2f position, int layers, int tileSize)
{
	HexagonGrid grid;

	tiles.resize(grid.getNumberOfTiles(layers));

	this->windowSize = windowSize;
	this->tileCount = 0;

	// Generate center hexagon.
	Shapes::hexagon(tiles[tileCount++].corners, 0, position, tileSize - 1);

	// Walk through every hexagon one layer at a time.
	for (int k = 1; k < layers; k++)
	{
		AxialCoordinates hexagon(-k, k);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < k; j++)
			{
				Shapes::hexagon(tiles[tileCount++].corners, 0, position + grid.getPosition(hexagon, tileSize), tileSize - 1);

				hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
			}
		}
	}
}

void FloorTiles::draw(sf::RenderWindow * window)
{
	for (int i = 0; i < tileCount; i++)
	{
		window->draw(tiles[i].corners, 6, PrimitiveType::TrianglesFan);
	}
}