#pragma once

#include "Entity.h"
#include "Enum.h"
#include "UpdateInfo.h"
#include "Shapes.h"
#include "Player.h"
#include "FloorTiles.h"
#include <SFML\Graphics.hpp>
#include <list>
#include <queue>

using std::list;
using std::queue;
using namespace sf;

class Territory
{
private:
	Rect<float> shape;
	Vertex border[7];
	queue<Entity *> spawnQueue;
	list<Entity *> entities;
	FloorTiles floorTiles;

public:
	bool active;

public:
	Territory(Vector2f position, int radius);
	~Territory();

	void addEntity(Entity * entity);
	void removeEntity(Entity * entity);
	void integrateSpawnQueue();

	void cleanup();
	void draw(RenderWindow * window);
	void update(UpdateInfo info);
};