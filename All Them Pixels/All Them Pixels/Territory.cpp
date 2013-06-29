#include "Territory.h"

Territory::Territory() { }

Territory::Territory(Vector2f position, float radius, World * world)
{
	float hexagonRadius = 10;
	float hexagonWidth = hexagonRadius * 2;
	float hexagonHeight = sqrt(3)/2 * hexagonWidth;
	int numberOfLayersHorizontal = (((radius * 2) - hexagonWidth) / (hexagonWidth * 1.5f)) + 1;
	int numberOfLayersVertical = ((((sqrt(3) / 2) *  radius * 2) / hexagonHeight) - 1) / 2;
	int layers = (numberOfLayersHorizontal < numberOfLayersVertical) ? numberOfLayersHorizontal : numberOfLayersVertical;
	HexagonGrid grid(Hexagon::FlatTopped);
	
	this->active = false;
	this->position = position;
	this->radius = radius;
	this->world = world;
	this->player = NULL;
	this->offset.x = layers;
	this->offset.y = layers;
	this->matrixLength = (layers * 2) + 1;

	gridMatrix = grid.generateGrid(position, hexagonRadius, layers);
	borderCoordinates = grid.getRingCoordinates(layers);

	for (int i = 0; i < borderCoordinates.size(); i++)
	{
		gridMatrix[offset.x + borderCoordinates[i].q][offset.y + borderCoordinates[i].r]->setColor(Color(255, 0, 0));
	}
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
	for (int i = 0; i < matrixLength; i++)
	{
		for (int j = 0; j < matrixLength; j++)
		{
			if (gridMatrix[i][j] != NULL)
			{
				if (Collision::isWithinWindow(gridMatrix[i][j]->getBoundingBox(), window))
					gridMatrix[i][j]->draw(window);
			}
		}
	}

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		if (Collision::isWithinWindow((*it)->getBoundingBox(), window))
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
			if (Collision::isClose(player, (*it)))
			{
				player->modHP(-((Projectile*)(*it))->getDamage()); //DIE();
				player->fade();
				(*it)->expend();
			}
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
						//projectile->expend();

						break;
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
		for (int i = 0; i < borderCoordinates.size(); i++)
		{
			Hexagon * hexagon = gridMatrix[offset.x + borderCoordinates[i].q][offset.y + borderCoordinates[i].r];

			if (Collision::hitBoxesOverlap(hexagon->getBoundingBox(), player->getBoundingBox()))
			{
				Vector2f direction = Vector2fMath::unitVector(player->getPosition() - position);
				Vector2f seekPosition = position + (direction * radius * 1.5f);
				std::list<Vector2f> penetration;

				if (SAT::collides(player->getConvexHull(), hexagon->getConvexHull(), penetration))
				{
					//player->translate(SAT::getShortestPenetration(penetration));

					world->changeTerritory(seekPosition);

					break;
				}				
			}
		}
	}

	// Spawn enemies
	if (rand() % 5 == 1)
	{
		Enemy * enemy = new Enemy(100, position + Vector2f(512 - rand() % 1024, 512 - rand() % 1024));
	
		addEntity(enemy);
	}
}