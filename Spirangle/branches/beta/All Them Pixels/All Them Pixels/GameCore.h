#pragma once

#include "Entity.h"
#include "UpdateInfo.h"
#include <SFML\Graphics.hpp>
#include <list>
#include <queue>

using std::list;
using std::queue;

using namespace sf;

class GameCore
{
private:
	queue<Entity *> spawnQueue;
	list<Entity *> entities;

public:
	~GameCore();

	void addEntity(Entity * entity);
	void removeEntity(Entity * entity);
	void integrateSpawnQueue();

	void update(UpdateInfo info);
	void draw(RenderWindow * window);
};