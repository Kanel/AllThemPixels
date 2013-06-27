#include "World.h"

World::World()
{
	Vector2f position(0, 0);
	HexagonGrid grid(HexagonStyle::PointyTopped);

	territoryRadius = 256;
	territorySpacing = 0;
	layers = 3;
	tileSize = territoryRadius + (territorySpacing / 2);
	matrixSize = (layers * 2) - 1;
	offset = Vector2i(layers - 1, layers - 1);

	territories = new Territory**[matrixSize];

	// Matrix
	for (int i = 0; i < matrixSize; i++)
	{
		territories[i] = new Territory*[matrixSize];

		for (int j = 0; j < matrixSize; j++)
		{
			territories[i][j] = NULL;
		}
	}

	territoryCount = 0;

	// Center Territory
	territories[offset.x][offset.y] = new Territory(position, territoryRadius, this);

	// Layer Territories
	for (int k = 1; k < layers; k++)
	{
		AxialCoordinates hexagon(-k, k);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < k; j++)
			{
				territories[offset.x + hexagon.q][offset.y + hexagon.r] = new Territory(position + grid.getPosition(hexagon, tileSize), territoryRadius, this);

				hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
			}
		}
	}
}

Territory * World::getTerritory(AxialCoordinates coordinates)
{
	return territories[offset.x + coordinates.q][offset.y + coordinates.r];
}

void World::changeTerritory(Vector2f position)
{
	HexagonGrid grid(HexagonStyle::PointyTopped);
	AxialCoordinates nextCoordinates = grid.getAxialCoordinates(position, tileSize);
	Territory * current = getTerritory(currentTerritoryCoordinates);
	Territory * next = getTerritory(nextCoordinates);
	Player * player = current->player;

	current->player = NULL;
	next->player = player;

	current->removeEntity(player);
	next->addEntity(player);

	Vector2f what = next->getPosition();

	player->setPosition(next->getPosition());

	currentTerritoryCoordinates = nextCoordinates;
}

void World::draw(RenderWindow * window)
{
	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (territories[i][j] != NULL)
			{
				territories[i][j]->draw(window);
			}
		}
	}
}

void World::update(UpdateInfo info)
{
	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (territories[i][j] != NULL)
			{
				territories[i][j]->integrateSpawnQueue();
				territories[i][j]->update(info);
				territories[i][j]->cleanup();
			}
		}
	}
}