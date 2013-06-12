#include "HexagonGrid.h"

int HexagonGrid::getNumberOfTiles(int layers)
{
	return (layers == 1) ? 1 : getNumberOfTiles(layers - 1) + (layers - 1) * 6;
}

int HexagonGrid::getNumberOfTilesInLayer(int layer)
{
	return (layer > 1) ? (layer - 1) * 6 : 1;
}

HexagonGrid::HexagonGrid(sf::Vector2f position, int layers, int tileSize, int tileSpacing)
{
	tiles = getNumberOfTiles(layers);
	shapes = new Hexagon[tiles];

	for (int i = 0, j = 1, d = 0; j <= layers; j++, d += tileSize + tileSpacing)
	{
		float l = 0;

		for (int k = 0; k < getNumberOfTilesInLayer(j); i++, k++, l += 2 * M_PI / getNumberOfTilesInLayer(j))
		{
			sf::Vector2f tilePosition = position;

			tilePosition.x += cosf(l + M_PI_2) * d;
			tilePosition.y += sinf(l + M_PI_2) * d;

			shapes[i] = Hexagon(tilePosition, tileSize / 2);
		}
	}
}

HexagonGrid::~HexagonGrid()
{
	delete shapes;
}

void HexagonGrid::draw(sf::RenderWindow * window)
{
	for (int i = 0; i < tiles; i++)
	{
		window->draw(shapes[i].getShape());
	}
}