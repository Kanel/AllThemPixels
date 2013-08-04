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
#include "EnemySpawner.h"
#include "ParticleSystem.h"
#include "Helper.h"
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
	struct Probe
	{
		float speed;
		Vector2f position;
		Vector2f direction;
	};

private:
	bool active;
	float radius;
	float hexagonRadius;
	int layers;
	Vector2f position;
	queue<Entity *> spawnQueue;
	list<Projectile *> enemyProjectiles;
	list<Projectile *> playerProjectiles;
	list<Enemy *> enemies;
	list<Effect *> effects;
	HexagonGridStorage floorTiles;
	vector<AxialCoordinates> borderCoordinates;
	World * world;
	Rect<float> boundingBox;
	vector<Hexagon *> safeZonesTiles[6];
	unordered_map<Uint32, list<Hexagon *>> partitionedSafeZonesTiles[6];
	vector<AxialCoordinates> spawnGrid;
	VertexCluster tileCluster;
	VertexCluster entityCluster;
	VertexCluster fadeTileCluster;
	Sounds * sounds;
	vector<Probe> probes;
	HexagonGridStorage fadeTiles;
	EnemySpawner enemySpawner;

private:
	void colorTiles(HexagonGridStorage &storage, Color base);
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
	void prepareProbes(Vector2f center);
	void updateProbes(Color color, Vector2f center);
	void populateHexagonGrid(HexagonGridStorage &storage, VertexCollection * vertexSource);

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
	void fade(Color color, Vector2f center);

	Rect<float> getBoundingBox();

	void cleanup();
	virtual void draw(RenderTarget& target, RenderStates states) const;

	void update(UpdateInfo info, Sounds * sounds);
};