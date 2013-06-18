#include "Territory.h"

Territory::Territory(Vector2f position, int radius)
{
	shape.top = position.y - radius;
	shape.left = position.x - radius;
	shape.width = radius * 2;
	shape.height = radius * 2;

	Shapes::rectangle(border, 0, position, radius * 2, radius * 2);

	border[4] = border[0];
}

Territory::~Territory()
{
	integrateSpawnQueue();

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end();)
	{
		Entity * entity = *it;

		it = entities.erase(it);

		delete entity;
	}
}

void Territory::addEntity(Entity * entity)
{
	spawnQueue.push(entity);
}

void Territory::removeEntity(Entity * entity)
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

void Territory::integrateSpawnQueue()
{
	while(spawnQueue.size() > 0)
	{
		entities.push_back(spawnQueue.front());
		spawnQueue.pop();
	}
}

void Territory::cleanup()
{
	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end();)
	{
		if (!Shapes::contains(shape, (*it)->getBoundingBox()))
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

void Territory::draw(RenderWindow * window)
{
	window->draw(border, 5, PrimitiveType::LinesStrip);

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		(*it)->draw(window);
	}
}

void Territory::update(UpdateInfo info)
{
	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		(*it)->update(info);
	}
}