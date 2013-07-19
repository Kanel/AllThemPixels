#include "EnemySpawner.h"
/*
Vector2f EnemySpawner::getSpawnLocation()
{
	bool found = false;	
	HexagonGrid grid(Hexagon::FlatTopped);
	AxialCoordinates origin = grid.getAxialCoordinates(player->getPosition() - position, hexagonRadius);
	Vector2f spawnPosition = position;

	while(!found)
	{
		int i = rand() % spawnGrid.size();
		int q = origin.q + spawnGrid[i].q;
		int r = origin.r + spawnGrid[i].r;
		int x = offset.x + q;
		int y = offset.y + r;

		if (0 <= x && x < matrixLength && 0 <= y && y < matrixLength)
		{
			if (gridMatrix[x][y] != NULL)
			{
				found = true;
				spawnPosition += grid.getPosition(AxialCoordinates(q, r), hexagonRadius);
			}
		}
	}
	return spawnPosition;
}

*/

/*
	// Spawn enemies
	Enemy * enemy = new Enemy(100, getSpawnLocation(), entityCluster.getCollection(1));

	enemy->educate(aiProperties[0]);
	enemy->arm(enemyWeapons[rand() % 4]);
	addEntity(enemy);
*/