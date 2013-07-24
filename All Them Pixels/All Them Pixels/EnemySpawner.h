#pragma once

#include "Enemy.h"
#include "UpdateInfo.h"
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

class EnemySpawner
{
private:
	int lastSpawnEvent;
	int spawnPointsUsed;

	SpawnConfiguration config;

public:
	EnemySpawner(SpawnConfiguration config);

	bool canSpawn();

	void update(vector<Vector2f> spawnPoints, queue<Entity *> &spawnQueue, VertexCollection * enemyVertexSource, VertexCollection * projectileVertexSource, UpdateInfo info);
};