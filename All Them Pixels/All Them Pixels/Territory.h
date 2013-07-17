#pragma once

#include "Entity.h"
#include "Flag.h"
#include "UpdateInfo.h"
#include "Shapes.h"
#include "Player.h"
#include "Enemy.h"
#include "UserInput.h"
#include "AI.h"
#include "Weapon.h"
#include "Collision.h"
#include "World.h"
#include "HexagonGrid.h"
#include "VertexCluster.h"
#include "Config.h"
#include "FloorTile.h"
#include <SFML/Graphics.hpp>
#include <list>
#include <queue>
#include <unordered_map>

using std::list;
using std::queue;
using std::unordered_map;
using namespace sf;

// Is this a sign of something?
class AI;
class Player;
class World;
class Projectile;
class Enemy;
class Weapon;
class VertexCluster;

class Territory
{
private:
	Vector2f position;
	float radius;
	queue<Entity *> spawnQueue;
	list<Projectile *> enemyProjectiles;
	list<Projectile *> playerProjectile;
	list<Enemy *> enemies;
	int matrixLength;
	Vector2i offset;
	FloorTile *** gridMatrix;
	vector<AxialCoordinates> borderCoordinates;
	World * world;
	Rect<float> boundingBox;
	vector<FloorTile *> safeZonesTiles[6];
	unordered_map<Uint32, list<FloorTile *>> partitionedSafeZonesTiles[6];
	vector<AxialCoordinates> spawnGrid;
	float hexagonRadius;
	VertexCluster tileCluster;
	VertexCluster entityCluster;

	Weapon enemyWeapons[4];
	AIProperties aiProperties[4];

private:
	void prepareSafeZoneTiles(int tileGridLayers);
	void spatialPartitionSafeRoomTiles();

public:
	bool active;
	Player * player;

public:
	Territory();
	Territory(Vector2f position, float radius, World * world);
	~Territory();
	queue<Entity *> *getSpawnQueue();

	Vector2f getPosition();

	void addEntity(Entity * entity);
	void removeEntity(Entity * entity);
	void integrateSpawnQueue();

	Vector2f getSpawnLocation();

	Rect<float> getBoundingBox();

	void cleanup();
	void draw(RenderWindow * window);
	void update(UpdateInfo info);
};