#include "Territory.h"

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
	int numberOfTiles;
	HexagonGrid grid(Hexagon::FlatTopped);

	numberOfTiles = grid.getNumberOfTiles(layers);

	tileCluster.create(8, numberOfTiles, 1, PrimitiveType::TrianglesStrip);
	entityCluster.create(VertexCluster::HexagonSource);
	entityCluster.create(VertexCluster::RectangleSource);

	this->hexagonRadius= hexagonRadius;
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

	gridMatrix = grid.generateGrid(position, hexagonRadius, layers, tileCluster.getCollection(0));
	borderCoordinates = grid.getRingCoordinates(layers);
	layers = 47;
	spawnGrid = grid.getRingCoordinates(layers + 2);

	for (int i = 0; i < borderCoordinates.size(); i++)
	{
		gridMatrix[offset.x + borderCoordinates[i].q][offset.y + borderCoordinates[i].r]->setColor(Color(255, 0, 0));
	}

	// Pre compute values!?!
	int index = 0;	
	
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

	WeaponConfiguration wc;
	wc.cooldown = 100;
	wc.damage = 20;
	wc.piercing = 1;
	wc.speed = 8;
	wc.spread = 4;
	wc.ttl = 20;
	enemyWeapons[0] = Weapon(wc, entityCluster.getCollection(0));
	enemyWeapons[1] = Weapon(wc, entityCluster.getCollection(0));
	enemyWeapons[2] = Weapon(wc, entityCluster.getCollection(0));
	enemyWeapons[3] = Weapon(wc, entityCluster.getCollection(0));

	aiProperties[0] = AI::generate(rand() % 5);
	aiProperties[1] = AI::generate(rand() % 5);
	aiProperties[2] = AI::generate(rand() % 5);
	aiProperties[3] = AI::generate(rand() % 5);
}

Territory::~Territory()
{
	// Todo: fix, does not work now
	integrateSpawnQueue();

	/*for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end();)
	{
		Entity * entity = *it;

		it = entities.erase(it);

		delete entity;
	}*/
}

queue<Entity *> *Territory::getSpawnQueue()
{
	return &spawnQueue;
}

Vector2f Territory::getPosition()
{
	return position;
}

void Territory::addEntity(Entity * entity)
{
	spawnQueue.push(entity);
}

// Note: Does not delete entities. Should it?
void Territory::removeEntity(Entity * entity)
{
	if(Enum::isFlagSet(entity->getType(), EntityTypes::EnemyProjectileEntity))
	{
		for (std::list<Projectile *>::iterator it = enemyProjectiles.begin(); it != enemyProjectiles.end();)
		{
			if ((*it) == entity)
			{
				enemyProjectiles.erase(it);

				break;
			}
			else
			{
				it++;
			}
		}
	}
	else if(Enum::isFlagSet(entity->getType(), EntityTypes::ProjectileEntity))
	{
		for (std::list<Projectile *>::iterator it = playerProjectile.begin(); it != playerProjectile.end();)
		{
			if ((*it) == entity)
			{
				playerProjectile.erase(it);

				break;
			}
			else
			{
				it++;
			}
		}
	}
	else if(Enum::isFlagSet(entity->getType(), EntityTypes::EnemyEntity))
	{
		for (std::list<Enemy *>::iterator it = enemies.begin(); it != enemies.end();)
		{
			if ((*it) == entity)
			{
				enemies.erase(it);

				break;
			}
			else
			{
				it++;
			}
		}
	}
}

void Territory::integrateSpawnQueue()
{
	while(spawnQueue.size() > 0)
	{
		Entity * entity = spawnQueue.front();

		if(Enum::isFlagSet(entity->getType(), EntityTypes::EnemyProjectileEntity))
		{
			enemyProjectiles.push_back((Projectile *)entity);
		}
		else if(Enum::isFlagSet(entity->getType(), EntityTypes::ProjectileEntity))
		{
			playerProjectile.push_back((Projectile *)entity);
		}
		else if(Enum::isFlagSet(entity->getType(), EntityTypes::EnemyEntity))
		{
			enemies.push_back((Enemy *)entity);
		}
		spawnQueue.pop();
	}
}

Vector2f Territory::getSpawnLocation()
{
	bool found = false;	
	HexagonGrid grid(Hexagon::FlatTopped);
	AxialCoordinates origin = grid.getAxialCoordinates(player->getPosition() - position, hexagonRadius);
	Vector2f spawnPosition = position;

	while(!found)
	{
		int i = rand() % spawnGrid.size();
		int q = origin.q + spawnGrid[i].q;
		int r = origin.r + spawnGrid[i].r;
		int x = offset.x + q;
		int y = offset.y + r;

		if (0 <= x && x < matrixLength && 0 <= y && y < matrixLength)
		{
			if (gridMatrix[x][y] != NULL)
			{
				found = true;
				spawnPosition += grid.getPosition(AxialCoordinates(q, r), hexagonRadius);
			}
		}
	}
	return spawnPosition;
}

Rect<float> Territory::getBoundingBox()
{
	return boundingBox;
}

void Territory::cleanup()
{
	for (std::list<Projectile *>::iterator it = enemyProjectiles.begin(); it != enemyProjectiles.end();)
	{
		Projectile * entity = *it;

		if (entity->isExpended())
		{
			it = enemyProjectiles.erase(it);
			delete entity;
		}
		else
		{
			it++;
		}
	}

	for (std::list<Projectile *>::iterator it = playerProjectile.begin(); it != playerProjectile.end();)
	{
		Projectile * entity = *it;

		if (entity->isExpended())
		{
			it = playerProjectile.erase(it);
			delete entity;
		}
		else
		{
			it++;
		}
	}

	for (std::list<Enemy *>::iterator it = enemies.begin(); it != enemies.end();)
	{
		Enemy * entity = *it;

		if (entity->isExpended())
		{
			it = enemies.erase(it);
			delete entity;
		}
		else
		{
			it++;
		}
	}

	if (active && player->isExpended())
	{
		delete player;

		active = false;
		player = NULL;
	}
}

void Territory::draw(RenderWindow * window)
{	
	window->draw(tileCluster);
	window->draw(entityCluster);

	if (active)
	{
		player->draw(window);
	}
}

void Territory::update(UpdateInfo info)
{
	if (active)
	{
		for (std::list<Projectile *>::iterator it = enemyProjectiles.begin(); it != enemyProjectiles.end(); it++)
		{
			(*it)->update(info);

			if (Collision::isClose(player, (*it)))
			{
				player->modHP(-((Projectile*)(*it))->getDamage()); //DIE();
				player->fade();
				(*it)->expend();
			}
		}
	}

	for (std::list<Projectile *>::iterator it = playerProjectile.begin(); it != playerProjectile.end(); it++)
	{
		(*it)->update(info);

		for (std::list<Enemy *>::iterator itEnemies = enemies.begin(); itEnemies != enemies.end(); itEnemies++)
		{
			if (Collision::isClose(*itEnemies, *it))
			{
				(*itEnemies)->modHP(-(*it)->getDamage());
				(*it)->expend();
			}
		}
	}
	for (std::list<Enemy *>::iterator it = enemies.begin(); it != enemies.end(); it++)
	{
		AI::update(&spawnQueue, *it, player, info);
	}

	if (active)
	{
		player->update(info);

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
					world->changeTerritory(seekPosition);

					break;
				}
			}
		}
	}

	// Spawn enemies
	if (active)
	{
		Enemy * enemy = new Enemy(100, getSpawnLocation(), entityCluster.getCollection(1));

		enemy->educate(aiProperties[0]);
		enemy->arm(enemyWeapons[rand() % 4]);
		addEntity(enemy);
	}
}