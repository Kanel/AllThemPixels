#include "Territory.h"

Territory::Territory() { }

Territory::Territory(Vector2f position, float radius, World * world)
{
	float hexagonRadius = 10;
	float hexagonWidth = hexagonRadius * 2;
	float hexagonHeight = sqrt(3)/2 * hexagonWidth;
	int numberOfLayersHorizontal = (((radius * 2) - hexagonWidth) / (hexagonWidth * 1.5f)) + 1;
	int numberOfLayersVertical = (((radius * 2) / hexagonHeight) / 2) + 1;
	int layers = (numberOfLayersHorizontal < numberOfLayersVertical) ? numberOfLayersHorizontal : numberOfLayersVertical;

	this->position = position;
	this->radius = radius;
	this->world = world;
	this->player = NULL;

	floorTiles = FloorTiles(Vector2i(2 * radius, 2 * radius), position, layers, hexagonRadius);
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

Vector2f Territory::getPosition()
{
	return position;
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
			entities.erase(it);

			break;
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
	float threshold = radius * 0.8;
	vector<Vector2f> vertecies;
	bool clean = false;

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end();)
	{
		clean = false;
		Entity * entity = *it;

		if (entity->isExpended())
		{
			clean = true;
		}
		/*else if (!Enum::isFlagSet(entity->getType(), EntityTypes::ProjectileEntity))
		{
			float distanceToCenter = Vector2fMath::distance(position, entity->getPosition());

			if (distanceToCenter >= threshold && !entity->collidesWith(convexHull))
			{
				clean = true;
			}
		}*/

		if (clean)
		{
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
	floorTiles.draw(window);

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		(*it)->draw(window);
	}
}

void Territory::update(UpdateInfo info)
{
	Vector2f vector = UserInput::getJoystickVector(0, Joystick::Axis::X, Joystick::Axis::Y);

	vector.x = (powf(vector.x, 2) > 400) ? -vector.x / 15 : 0;
	vector.y = (powf(vector.y, 2) > 400) ? -vector.y / 15 : 0;

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		(*it)->update(info);

		if(Enum::isFlagSet((*it)->getType(), EntityTypes::EnemyProjectileEntity))
		{
			if (Collision::isClose(player, (*it))) player->modHP(-((Projectile*)(*it))->getDamage()); //DIE();
			continue;
		}
		if(Enum::isFlagSet((*it)->getType(), EntityTypes::ProjectileEntity))
		{
			Projectile* projectile = (Projectile*)*it;
			for (std::list<Entity *>::iterator it2 = entities.begin(); it2 != entities.end(); it2++) //can has enemy list?
			{
				if(Enum::isFlagSet((*it2)->getType(), EntityTypes::EnemyEntity))
				{
					if (Collision::isClose((*it2), projectile))
					{
						((Enemy *)(*it2))->modHP(-projectile->getDamage());
						continue;
					}
				}
			}
			continue;
		}
		if (Enum::isFlagSet((*it)->getType(), EntityTypes::EnemyEntity))
		{
			AI::update(this, (Enemy *)(*it), player, info);
			continue;
		}

	}

	if (player != NULL)
	{
		if (floorTiles.intersectsBorder(player))
		{
			Vector2f direction = Vector2fMath::unitVector(player->getPosition() - position);
			Vector2f seekPosition = position + (direction * radius * 1.5f);

			world->changeTerritory(seekPosition);
		}
	}

	// Spawn enemies
	if (rand() % 5 == 1)
	{
		Enemy * enemy = new Enemy(100, position + Vector2f(512 - rand() % 1024, 512 - rand() % 1024));
	
		//addEntity(enemy);
	}
}