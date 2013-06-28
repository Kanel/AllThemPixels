#include "FloorTiles.h"
#include "Shapes.h"

FloorTiles::FloorTiles() { }

FloorTiles::FloorTiles(Vector2i windowSize, Vector2f position, int layers, int tileSize)
{
	int borderLayer = layers - 1;
	int tileDimension = tileSize - 1;
	HexagonGrid grid(Hexagon::FlatTopped);
	AxialCoordinates hexagon;

	tiles.resize(grid.getNumberOfTiles(layers));
	border.resize(grid.getNumberOfTilesInLayer(layers + 1));

	this->windowSize = windowSize;
	this->tileCount = 0;
	this->borderCount = 0;

	// Generate center hexagon.
	Shapes::hexagon(tiles[tileCount++].corners, 0, position, tileDimension, true);

	// Walk through every hexagon one layer at a time.
	for (int k = 1; k < borderLayer; k++)
	{
		hexagon = AxialCoordinates(-k, k);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < k; j++)
			{
				Vector2f hexagonPosition = position + grid.getPosition(hexagon, tileSize);

				tiles[tileCount].boundingBox = Rect<float>(hexagonPosition.x - tileDimension, hexagonPosition.y - tileDimension, tileDimension * 2, tileDimension * 2);

				Shapes::hexagon(tiles[tileCount++].corners, 0, hexagonPosition, tileDimension, true);

				hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
			}
		}
	}	

	// Generate border
	hexagon = AxialCoordinates(-borderLayer, borderLayer);

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < borderLayer; j++)
		{
			Vector2f hexagonPosition = position + grid.getPosition(hexagon, tileSize);

			border[borderCount].boundingBox = Rect<float>(hexagonPosition.x - tileDimension, hexagonPosition.y - tileDimension, tileDimension * 2, tileDimension * 2);

			Shapes::hexagon(border[borderCount++].corners, 0, hexagonPosition, tileDimension, Color(255, 0, 0), true);

			hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
		}
	}
}

bool FloorTiles::intersectsBorder(Entity * entity)
{
	for (int i = 0; i < borderCount; i++)
	{
		if (Collision::hitBoxesOverlap(border[i].boundingBox, entity->getBoundingBox()))
		{
			// Todo: more accurate collision.

			return true;
		}
	}
	return false;
}

void FloorTiles::draw(sf::RenderWindow * window)
{
	for (int i = 0; i < tileCount; i++)
	{
		window->draw(tiles[i].corners, 6, PrimitiveType::TrianglesFan);
	}

	for (int i = 0; i < borderCount; i++)
	{
		window->draw(border[i].corners, 6, PrimitiveType::TrianglesFan);
	}
}