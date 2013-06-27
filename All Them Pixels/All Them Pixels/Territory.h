#pragma once

#include "Entity.h"
#include "Enum.h"
#include "UpdateInfo.h"
#include "Shapes.h"
#include "Player.h"
#include "Enemy.h"
#include "FloorTiles.h"
#include "UserInput.h"
#include "AI.h"
#include "Collision.h"
#include "World.h"
#include <SFML\Graphics.hpp>
#include <list>
#include <queue>

using std::list;
using std::queue;
using namespace sf;

class Player;
class World;

class Territory
{
private:
	Vector2f position;
	float radius;
	queue<Entity *> spawnQueue;
	list<Entity *> entities;
	FloorTiles floorTiles;
	World * world;

public:
	bool active;
	Player * player;

public:
	Territory();
	Territory(Vector2f position, float radius, World * world);
	~Territory();

	Vector2f getPosition();

	void addEntity(Entity * entity);
	void removeEntity(Entity * entity);
	void integrateSpawnQueue();

	void cleanup();
	void draw(RenderWindow * window);
	void update(UpdateInfo info);
};