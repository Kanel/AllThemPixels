#pragma once

#include "Entity.h"
#include "UpdateInfo.h"
#include "HexagonGrid.h"
#include <SFML\Graphics.hpp>
#include <list>
#include <queue>

using std::list;
using std::queue;
using namespace sf;

class Territory
{
private:
	HexagonGrid grid;
	queue<Entity *> spawnQueue;
	list<Entity *> entities;

public:
	bool active;

public:
	Territory(Vector2f position, int radius);
	~Territory();

	void addEntity(Entity * entity);
	void removeEntity(Entity * entity);
	void integrateSpawnQueue();

	void draw(RenderWindow * window);
	void update(UpdateInfo info);
};