#include "GameCore.h"

GameCore::~GameCore()
{
	integrateSpawnQueue();

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end();)
	{
		Entity * entity = *it;

		it = entities.erase(it);

		delete entity;
	}
}

void GameCore::addEntity(Entity * entity)
{
	spawnQueue.push(entity);
}

void GameCore::removeEntity(Entity * entity)
{
	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end();)
	{
		if ((*it) == entity)
		{
			Entity * entity = *it;

			it = entities.erase(it);

			delete entity;
		}
		else
		{
			it++;
		}
	}
}

void GameCore::integrateSpawnQueue()
{
	while(spawnQueue.size() > 0)
	{
		entities.push_back(spawnQueue.front());
		spawnQueue.pop();
	}
}

void GameCore::update(UpdateInfo info)
{
	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		(*it)->update(info);
	}
}

void GameCore::draw(RenderWindow * window)
{
	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		(*it)->draw(window);
	}
}