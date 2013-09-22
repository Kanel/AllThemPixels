#include "EnemySpawner.h"

int EnemySpawner::calculateValue(AIProperties aiProperties)
{
	int value = 0;

	value += aiProperties.aimAvoidanceDistance;
	value += aiProperties.aimAvoidanceSpeed;
	value += aiProperties.movementSpeed;
	value += abs(aiProperties.orbitalSpeed);
	value += aiProperties.orbitalThreshold;
	value += aiProperties.orbitalDistance;
	value += aiProperties.stalkingDistance;
	value += aiProperties.sinusRotationSpeed;
	value += aiProperties.sinusRotationRadius;

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

void EnemySpawner::update(vector<Vector2f> spawnPoints, queue<Entity *> &spawnQueue, Player * player, VertexCollection * enemyVertexSource, VertexCollection * projectileVertexSource, UpdateInfo info)
{
	if (lastSpawnEvent + config.spawnRate < info.elapsedGameTime)
	{
		if (spawnPointsUsed + 100 <= config.spawnPoints)
		{
			int spawnIndex = rand() % spawnPoints.size();
			EnemyClass enemyClass = enemyClasses[rand() % enemyClasses.size()];
			Enemy * enemy = new Enemy(100, spawnPoints[spawnIndex], new HexagonHull(spawnPoints[spawnIndex], sqrtf(enemyClass.points / 10), sqrtf(enemyClass.points) / 5, Hexagon::FlatTopped, enemyVertexSource));
			StalkingMovement * stalkingMovement = new StalkingMovement(player, enemyClass.aiProperties.movementSpeed, enemyClass.aiProperties.stalkingDistance);
			OrbitalMovement * orbital = new OrbitalMovement(player, enemyClass.aiProperties.orbitalSpeed, enemyClass.aiProperties.orbitalDistance, enemyClass.aiProperties.orbitalThreshold);
			SinusMovement * sinusMovement = new SinusMovement(M_PI / 1.0f, 25);
			AimAvoidanceMovement * aimAvoidance = new AimAvoidanceMovement(player, enemyClass.aiProperties.aimAvoidanceSpeed, enemyClass.aiProperties.aimAvoidanceDistance);

			enemy->equip(stalkingMovement);			
			enemy->equip(orbital);
			enemy->equip(sinusMovement);
			enemy->equip(aimAvoidance);

			enemy->educate(enemyClass.aiProperties);
			enemy->arm(Weapon(enemyClass.weaponconfiguration, projectileVertexSource));
			spawnQueue.push(enemy);

			spawnPointsUsed += enemyClass.points;
		}
	}
}