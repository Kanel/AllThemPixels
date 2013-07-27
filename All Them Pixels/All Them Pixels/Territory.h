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
#include "Sounds.h"
#include "World.h"
#include "HexagonGrid.h"
#include "VertexCluster.h"
#include "Config.h"
#include "FloorTile.h"
#include "EnemySpawner.h"
#include "ParticleSystem.h"
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

class Territory : public Drawable
{
private:
	bool active;
	float radius;
	int floorTilesLength;
	float hexagonRadius;
	Vector2f position;
	queue<Entity *> spawnQueue;
	list<Projectile *> enemyProjectiles;
	list<Projectile *> playerProjectiles;
	list<Enemy *> enemies;
	list<Effect *> effects;
	Vector2i offset;
	FloorTile *** floorTiles;
	vector<AxialCoordinates> borderCoordinates;
	World * world;
	Rect<float> boundingBox;
	vector<FloorTile *> safeZonesTiles[6];
	unordered_map<Uint32, list<FloorTile *>> partitionedSafeZonesTiles[6];
	vector<AxialCoordinates> spawnGrid;
	VertexCluster tileCluster;
	VertexCluster entityCluster;
	Sounds * sounds;

	EnemySpawner enemySpawner;

private:
	void colorFloorTiles();
	void colorBorderTiles();
	void colorSafeZoneTiles();
	void colorEnemyAuraTiles();
	void cleanFloorTiles();
	void prepareSafeZoneTiles(int tileGridLayers);	
	void spatialPartitionSafeRoomTiles();
	void updatePlayerProjectiles(UpdateInfo info);
	void updateEnemyProjectiles(UpdateInfo info);
	void updateEnemies(UpdateInfo info);
	void updatePlayer(UpdateInfo info);
	void updateEffects(UpdateInfo info);
	void updateBorderTiles();
	void borderControl();

public:	
	Player * player;

public:
	Territory();
	Territory(Vector2f position, float radius, World * world, SpawnConfiguration spawnConfig);
	~Territory();
	queue<Entity *> *getSpawnQueue();

	Vector2f getPosition();

	void addEntity(Entity * entity);
	void removeEntity(Entity * entity);
	void integrateSpawnQueue();

	vector<Vector2f> getSpawnPoints();

	void activate(Player * player);
	void deactivate();
	bool isActive();
	bool isCleared();

	Rect<float> getBoundingBox();

	void cleanup();
	virtual void draw(RenderTarget& target, RenderStates states) const;

	void update(UpdateInfo info, Sounds * sounds);
};