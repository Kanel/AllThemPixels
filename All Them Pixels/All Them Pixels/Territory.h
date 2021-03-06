#pragma once

#include "Entity.h"
#include "Flag.h"
#include "UpdateInfo.h"
#include "Shapes.h"
#include "Player.h"
#include "Enemy.h"
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
#include <thread>

using std::list;
using std::queue;
using std::unordered_map;
using std::thread;
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
	struct TileColoring
	{
		AxialCoordinates coordinate;
		Color previous;
	};
	struct FloorData
	{
		stack<TileColoring> &tileColorings;
		HexagonGridStorage<Hexagon *> &floorTiles;
	};

private:
	bool active;
	bool loaded;
	float radius;
	float hexagonRadius;
	int layers;
	Vector2f position;
	queue<Entity *> spawnQueue;
	list<Projectile *> enemyProjectiles;
	list<Projectile *> playerProjectiles;
	list<Enemy *> enemies;
	list<Effect *> effects;
	HexagonGridStorage<Hexagon *> floorTiles;
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
	HexagonGridStorage<Hexagon *> fadeTiles;
	EnemySpawner enemySpawner;
	stack<TileColoring> tileColorings;

private:
	void colorTiles(HexagonGridStorage<Hexagon *> &storage, Color base);
	void colorBorderTiles();
	void colorSafeZoneTiles();
	void colorEnemyAuraTiles();
	void cleanFloorTiles();
	void prepareSafeZoneTiles(int tileGridLayers);	
	void spatialPartitionSafeRoomTiles();
	void updatePlayerProjectiles(UpdateInfo info, Controls * controls);
	void collidePlayerProjectiles(UpdateInfo info, Controls * controls);
	void updateEnemyProjectiles(UpdateInfo info, Controls * controls);
	void collideEnemyProjectiles(UpdateInfo info, Controls * controls);
	void collideEnemyProjectilesSafe(UpdateInfo info, Controls * controls);
	void updateEnemies(UpdateInfo info);
	void updatePlayer(UpdateInfo info, Controls * controls);
	void updateEffects(UpdateInfo info);
	void collideBorderTiles();
	void prepareProbes(Vector2f center);
	void updateProbes(Color color, Vector2f center);
	void populateHexagonGrid(HexagonGridStorage<Hexagon *> &storage, VertexCollection * vertexSource);

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

	void load();
	void unLoad();
	bool isLoaded();

	Rect<float> getBoundingBox();

	void cleanup();
	virtual void draw(RenderTarget& target, RenderStates states) const;

	void update(UpdateInfo info, Controls * controls, Sounds * sounds);
};