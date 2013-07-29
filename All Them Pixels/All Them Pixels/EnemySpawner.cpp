#include "EnemySpawner.h"

int EnemySpawner::calculateValue(AIProperties aiProperties)
{
	int value = 0;

	value += aiProperties.aimavoidance;
	value += aiProperties.playerspace;
	value += (aiProperties.righthanded) ? 1 : 0;
	value += aiProperties.speed;

	return value;
}

int EnemySpawner::calculateValue(WeaponConfiguration weaponConfiguration)
{
	int value = 0;

	value += 1.0f / weaponConfiguration.cooldown;
	value += weaponConfiguration.damage;
	value += weaponConfiguration.piercing;
	value += weaponConfiguration.speed;
	value += weaponConfiguration.spread;
	value += 1.0f / weaponConfiguration.ttl;

	return value;
}

EnemySpawner::EnemySpawner(SpawnConfiguration config)
{
	this->config = config;

	lastSpawnEvent = 0;
	spawnPointsUsed = 0;

	for (int i = 0; i < 4; i++)
	{
		int aiIndex =  rand() % config.aiProperties.size();
		int weaponIndex =  rand() % config.enemyWeapons.size();
		EnemyClass enemyClass;
				
		enemyClass.points = calculateValue(config.aiProperties[aiIndex]);
		enemyClass.points += calculateValue(config.enemyWeapons[weaponIndex]);
		enemyClass.aiProperties = config.aiProperties[aiIndex];
		enemyClass.weaponconfiguration = config.enemyWeapons[weaponIndex];

		enemyClasses.push_back(enemyClass);
	}
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
			EnemyClass enemyClass = enemyClasses[rand() % enemyClasses.size()];
			Enemy * enemy = new Enemy(100, spawnPoints[spawnIndex], new HexagonHull(spawnPoints[spawnIndex], enemyClass.points / 10, sqrtf(enemyClass.points), Hexagon::FlatTopped, enemyVertexSource));
			
			enemy->educate(enemyClass.aiProperties);
			enemy->arm(Weapon(enemyClass.weaponconfiguration, projectileVertexSource));
			spawnQueue.push(enemy);

			spawnPointsUsed += enemyClass.points;
		}
	}
}