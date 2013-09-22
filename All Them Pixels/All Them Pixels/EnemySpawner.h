#pragma once

#include "Enemy.h"
#include "Player.h"
#include "UpdateInfo.h"
#include "StalkingMovement.h"
#include "SinusMovement.h"
#include "OrbitalMovement.h"
#include "AimAvoidanceMovement.h"
#include <queue>
#include <vector>

using std::queue;
using std::queue;

struct SpawnConfiguration
{
	int spawnPoints;
	int spawnRate;
	vector<AIProperties> aiProperties;
	vector<WeaponConfiguration> enemyWeapons;
};

struct EnemyClass
{
	int points;
	AIProperties aiProperties;
	WeaponConfiguration weaponconfiguration;
};

class EnemySpawner
{
private:
	int lastSpawnEvent;
	int spawnPointsUsed;
	vector<EnemyClass> enemyClasses;
	SpawnConfiguration config;

private:
	int calculateValue(AIProperties aiProperties);
	int calculateValue(WeaponConfiguration weaponConfiguration);

public:
	EnemySpawner(SpawnConfiguration config);

	bool canSpawn();

	void update(vector<Vector2f> spawnPoints, queue<Entity *> &spawnQueue, Player * player, VertexCollection * enemyVertexSource, VertexCollection * projectileVertexSource, UpdateInfo info);
};