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
	float originDistance = 2 * sqrtf(powf(tileSize / 2, 2) - powf(tileSize / 4, 2)) + tileSpacing;
	
	tiles = getNumberOfTiles(layers);
	shapes = new Hexagon[tiles];

	for (int i = 0, j = 1, d = 0; j <= layers; j++, d += originDistance)
	{
		float l =  M_PI_2 + (2 * 2 * M_PI / 6);	
		int stepsTaken = 0;
		sf::Vector2f current = position;
		sf::Vector2f step;

		current.x += cosf(M_PI_2) * d;
		current.y += sinf(M_PI_2) * d;
		step.x = cosf(l) * originDistance;
		step.y = sinf(l) * originDistance;

		for (int k = 0; k < getNumberOfTilesInLayer(j); i++, k++)
		{
			shapes[i] = Hexagon(current, tileSize / 2);

			current += step;
			stepsTaken++;

			if (stepsTaken == j - 1)
			{
				stepsTaken = 0;
				l += 2 * M_PI / 6;

				step.x = cosf(l) * originDistance;
				step.y = sinf(l) * originDistance;
			}
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