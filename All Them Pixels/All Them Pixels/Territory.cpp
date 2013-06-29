#include "Territory.h"

Territory::Territory() { }

Territory::Territory(Vector2f position, float radius, World * world)
{
	// Flat topped hexagons
	// Todo: clean it a bit
	// Todo: figure out number of layers
	float hexagonRadius = 10;
	float hexagonWidth = hexagonRadius * 2;
	float hexagonHeight = sqrt(3)/2 * hexagonWidth;
	int numberOfLayersHorizontal = (((radius * 2) - hexagonWidth) / (hexagonWidth * 1.5f)) + 1;
	int numberOfLayersVertical = ((((sqrt(3) / 2) *  radius * 2) / hexagonHeight) - 1) / 2;
	int layers = (numberOfLayersHorizontal < numberOfLayersVertical) ? numberOfLayersHorizontal : numberOfLayersVertical;
	HexagonGrid grid(Hexagon::FlatTopped);
	
	this->position = position;
	this->radius = radius;
	this->world = world;

	active = false;	
	player = NULL;
	offset.x = layers;
	offset.y = layers;
	matrixLength = (layers * 2) + 1;
	boundingBox.width = (layers * hexagonWidth * 1.5f) + hexagonWidth;
	boundingBox.height = ((layers * 2) + 1) * hexagonHeight;
	boundingBox.left = position.x - (boundingBox.width / 2);
	boundingBox.top = position.y - (boundingBox.height / 2);

	gridMatrix = grid.generateGrid(position, hexagonRadius, layers);
	borderCoordinates = grid.getRingCoordinates(layers);

	for (int i = 0; i < borderCoordinates.size(); i++)
	{
		gridMatrix[offset.x + borderCoordinates[i].q][offset.y + borderCoordinates[i].r]->setColor(Color(255, 0, 0));
	}

	// Pre compute values!?!
	int index = 0;
	
	layers = 47;
	
	drawGrid.resize(grid.getNumberOfTiles(layers));

	drawGrid[index++] = AxialCoordinates(offset.x, offset.y);

	// Layer Territories
	for (int k = 1; k <= layers; k++)
	{
		AxialCoordinates hexagon(offset.x + -k, offset.y + k);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < k; j++)
			{
				drawGrid[index++] = hexagon;
				hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
			}
		}
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

Rect<float> Territory::getBoundingBox()
{
	return boundingBox;
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
	if (active)
	{
		int layers = 47;
		HexagonGrid grid(Hexagon::FlatTopped);
		AxialCoordinates origin = grid.getAxialCoordinates(player->getPosition() - position, 10);

		gridMatrix[offset.x + origin.q][offset.y + origin.r]->draw(window);

		// Layer Territories
		for (int i = 0; i < drawGrid.size(); i++)
		{
			int q = origin.q + drawGrid[i].q;
			int r = origin.r + drawGrid[i].r;
					
			if (0 <= q && q < matrixLength && 0 <= r && r < matrixLength)
			{
				if (gridMatrix[q][r] != NULL)
				{
					gridMatrix[q][r]->draw(window);
				}		
			}
		}
	}
	else
	{
		for (int i = 0; i < matrixLength; i++)
		{
			for (int j = 0; j < matrixLength; j++)
			{
				if (gridMatrix[i][j] != NULL)
				{
					if (Collision::isWithinWindow(gridMatrix[i][j]->getBoundingBox(), window->getView()))
						gridMatrix[i][j]->draw(window);
				}
			}
		}
	}

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		if (Collision::isWithinWindow((*it)->getBoundingBox(), window->getView()))
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
						projectile->expend();

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
	
		//addEntity(enemy);
	}
}