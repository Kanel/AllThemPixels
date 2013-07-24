#include "EnemySpawner.h"

EnemySpawner::EnemySpawner(SpawnConfiguration config)
{
	this->config = config;

	lastSpawnEvent = 0;
	spawnPointsUsed = 0;
}

bool EnemySpawner::canSpawn()
{
	return spawnPointsUsed + 100 <= config.spawnPoints;
}

void EnemySpawner::update(vector<Vector2f> spawnPoints, queue<Entity *> &spawnQueue, VertexCollection * enemyVertexSource, VertexCollection * projectileVertexSource, UpdateInfo info)
{
	if (lastSpawnEvent + config.spawnRate < info.elapsedGameTime)
	{
		if (spawnPointsUsed + 100 <= config.spawnPoints)
		{
			int spawnIndex = rand() % spawnPoints.size();
			int aiIndex =  rand() % config.aiProperties.size();
			int weaponIndex =  rand() % config.enemyWeapons.size();
			Enemy * enemy = new Enemy(100, spawnPoints[spawnIndex], enemyVertexSource);

			enemy->educate(config.aiProperties[aiIndex]);
			enemy->arm(Weapon(config.enemyWeapons[weaponIndex], projectileVertexSource));
			spawnQueue.push(enemy);

			spawnPointsUsed += 100;
		}
	}
}