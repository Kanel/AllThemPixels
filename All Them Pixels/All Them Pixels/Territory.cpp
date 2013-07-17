#include "Territory.h"

void Territory::prepareSafeZoneTiles(int tileGridLayers)
{
	int origins[6][2]= {
		{ 0,				-tileGridLayers },
		{ tileGridLayers,	-tileGridLayers },
		{ tileGridLayers,	 0 },
		{ 0,				tileGridLayers },
		{-tileGridLayers,	tileGridLayers },
		{-tileGridLayers,	0 },
	};
	int layers = 10;	
	HexagonGrid grid(Hexagon::FlatTopped);

	for(int direction = 0; direction < 6; direction++)
	{
		int x = offset.x + origins[direction][0];
		int y = offset.y + origins[direction][1];
		AxialCoordinates hexagon(origins[direction][0], origins[direction][1]);

		gridMatrix[x][y]->setColor(Color(0, 255, 0));
		safeZonesTiles[direction].push_back(gridMatrix[x][y]);

		for (int k = 1; k <= layers; k++)
		{
			hexagon = AxialCoordinates(origins[direction][0] + -k, origins[direction][1] + k);

			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < k; j++)
				{
					x = offset.x + hexagon.q;
					y = offset.y + hexagon.r;

					if (0 <= x && x < matrixLength && 0 <= y && y < matrixLength)
					{
						if (gridMatrix[x][y] != NULL)
						{
							gridMatrix[x][y]->setColor(Color(0, 255, 0));
							safeZonesTiles[direction].push_back(gridMatrix[x][y]);
						}
					}
					hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
				}
			}
		}
	}
}

void Territory::spatialPartitionSafeRoomTiles()
{
	Rect<float> box = getBoundingBox();
	unordered_map<Uint32, list<Hexagon *>> map[6];

	for (int i = 0; i < 6; i++)
	{
		for (auto hexagon : safeZonesTiles[i])
		{
			Rect<float> boundingBox = hexagon->getBoundingBox();
			short startX = floor(boundingBox.left / TERRITORY_CELL_SIZE);
			short endX = ceil((boundingBox.left + boundingBox.width) / TERRITORY_CELL_SIZE);
			short startY = floor(boundingBox.top / TERRITORY_CELL_SIZE);
			short endY = ceil((boundingBox.top + boundingBox.height) / TERRITORY_CELL_SIZE);

			for (short a = startX; a <= endX; a++)
			{
				for (short b = startY; b <= endY; b++)
				{
					Uint32 composite = (a << 16) | b;

					partitionedSafeZonesTiles[i][composite].push_back(hexagon);
				}
			}
		}
	}
}

Territory::Territory(Vector2f position, float radius, World * world)
{
	// Flat topped hexagons
	// Todo: clean it a bit
	// Todo: figure out number of layers
	float hexagonWidth;
	float hexagonHeight;
	int numberOfLayersHorizontal;
	int numberOfLayersVertical;
	int layers;
	int spawnRingSize;
	HexagonGrid grid(Hexagon::FlatTopped);
	WeaponConfiguration wc;

	this->hexagonRadius = TERRITORY_TILE_SIZE;
	this->position = position;
	this->radius = radius;
	this->world = world;

	hexagonWidth = hexagonRadius * 2;
	hexagonHeight = sqrt(3)/2 * hexagonWidth;
	numberOfLayersHorizontal = (((radius * 2) - hexagonWidth) / (hexagonWidth * 1.5f)) + 1;
	numberOfLayersVertical = ((((sqrt(3) / 2) *  radius * 2) / hexagonHeight) - 1) / 2;
	layers = (numberOfLayersHorizontal+numberOfLayersVertical) / 2;//(numberOfLayersHorizontal < numberOfLayersVertical) ? numberOfLayersHorizontal : numberOfLayersVertical;
	spawnRingSize = 47; //+2?

	active = false;	
	player = NULL;
	offset.x = layers;
	offset.y = layers;
	matrixLength = (layers * 2) + 1;
	boundingBox.width = (layers * hexagonWidth * 1.5f) + hexagonWidth;
	boundingBox.height = ((layers * 2) + 1) * hexagonHeight;
	boundingBox.left = position.x - (boundingBox.width / 2);
	boundingBox.top = position.y - (boundingBox.height / 2);

	tileCluster.create(8, grid.getNumberOfTiles(layers), 1, PrimitiveType::TrianglesStrip);
	entityCluster.create(VertexCluster::HexagonSource);
	entityCluster.create(VertexCluster::RectangleSource);

	gridMatrix = FloorTile::generateGrid(position, hexagonRadius, layers, tileCluster.getCollection(0));
	borderCoordinates = grid.getRingCoordinates(layers);
	spawnGrid = grid.getRingCoordinates(spawnRingSize);

	for (int i = 0; i < borderCoordinates.size(); i++)
	{
		gridMatrix[offset.x + borderCoordinates[i].q][offset.y + borderCoordinates[i].r]->setColor(Color(255, 0, 0));
	}

	prepareSafeZoneTiles(layers);
	spatialPartitionSafeRoomTiles();

	wc.cooldown = 500;
	wc.damage = 20;
	wc.piercing = 1;
	wc.speed = 8;
	wc.spread = 4;
	wc.ttl = 500;

	enemyWeapons[0] = Weapon(wc, entityCluster.getCollection(0));
	enemyWeapons[1] = Weapon(wc, entityCluster.getCollection(0));
	enemyWeapons[2] = Weapon(wc, entityCluster.getCollection(0));
	enemyWeapons[3] = Weapon(wc, entityCluster.getCollection(0));

	aiProperties[0] = AI::generate((rand() % 5) + 1);
	aiProperties[1] = AI::generate((rand() % 5) + 1);
	aiProperties[2] = AI::generate((rand() % 5) + 1);
	aiProperties[3] = AI::generate((rand() % 5) + 1);
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
	if(Flag::isFlagSet(entity->getType(), EnemyProjectileEntity))
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
	else if(Flag::isFlagSet(entity->getType(), ProjectileEntity))
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
	else if(Flag::isFlagSet(entity->getType(), EnemyEntity))
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

		if(Flag::isFlagSet(entity->getType(), EnemyProjectileEntity))
		{
			enemyProjectiles.push_back((Projectile *)entity);
		}
		else if(Flag::isFlagSet(entity->getType(), ProjectileEntity))
		{
			playerProjectile.push_back((Projectile *)entity);
		}
		else if(Flag::isFlagSet(entity->getType(), EnemyEntity))
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
	if (!player->getIsInSafeZone())
	{
		// Update all player projectiles
		for (auto projectile : playerProjectile)
		{
			projectile->update(info);

			for (auto enemy : enemies)
			{
				if (Collision::isClose(enemy, projectile))
				{
					enemy->modHP(-projectile->getDamage());
					projectile->expend();

					if (enemy->isExpended())
					{
						player->addSkillPoints(enemy->getSkillPoints());
					}
				}
			}
		}

		// Update all enemies.
		for (auto enemy : enemies)
		{
			AI::update(&spawnQueue, enemy, player, info);
		}
	}

	// Check if the player is hit by projectiles.
	for (auto projectile : enemyProjectiles)
	{
		projectile->update(info);

		if (Collision::isClose(player, projectile))
		{
			player->modHP(-projectile->getDamage()); //DIE();
			player->fade();
			projectile->expend();			
		}

		if (player->getIsInSafeZone())
		{
			Rect<float> boundingBox = projectile->getBoundingBox();
			int safeZoneIndex = player->getSafeZoneIndex();
			short startX = floor(boundingBox.left / TERRITORY_CELL_SIZE);
			short endX = ceil((boundingBox.left + boundingBox.width) / TERRITORY_CELL_SIZE);
			short startY = floor(boundingBox.top / TERRITORY_CELL_SIZE);
			short endY = ceil((boundingBox.top + boundingBox.height) / TERRITORY_CELL_SIZE);

			for (short a = startX; a <= endX; a++)
			{
				for (short b = startY; b <= endY; b++)
				{
					Uint32 composite = (a << 16) | b;
					auto cell = partitionedSafeZonesTiles[safeZoneIndex].find(composite);

					if (cell != partitionedSafeZonesTiles[safeZoneIndex].end())
					{
						for (auto hexagon : cell->second)
						{
							if (Collision::hitBoxesOverlap(hexagon->getBoundingBox(), projectile->getBoundingBox()))
							{
								std::list<Vector2f> penetration;

								if (SAT::collides(projectile->getConvexHull(), hexagon->getConvexHull(), penetration))
								{
									projectile->expend();

									break;
								}
							}
						}
					}
				}
			}
		}
	}

	// Player update and safe zone checks.
	player->update(info);
	player->setIsInSafeZone(false);

	for (int i = 0; i < 6 && !player->getIsInSafeZone(); i++)
	{
		for (int j = 0; j < safeZonesTiles[i].size(); j++)
		{
			FloorTile * tile = safeZonesTiles[i][j];

			if (Collision::hitBoxesOverlap(tile->getBoundingBox(), player->getBoundingBox()))
			{
				std::list<Vector2f> penetration;

				if (SAT::collides(player->getConvexHull(), tile->getConvexHull(), penetration))
				{
					player->setIsInSafeZone(true);
					player->setSafeZoneIndex(i);

					break;
				}
			}
		}
	}

	// Spawn enemies
	Enemy * enemy = new Enemy(100, getSpawnLocation(), entityCluster.getCollection(1));

	enemy->educate(aiProperties[0]);
	enemy->arm(enemyWeapons[rand() % 4]);
	addEntity(enemy);
	
	// Background tile stuff.
	HexagonGrid grid(Hexagon::FlatTopped);

	for (int i = 0; i < matrixLength; i++)
	{
		for (int j = 0; j < matrixLength; j++)
		{
			if (gridMatrix[i][j] != NULL)
			{
				gridMatrix[i][j]->resetColor();
			}
		}
	}
	for (auto enemy : enemies)
	{
		AxialCoordinates coordinates = grid.getAxialCoordinates(enemy->getPosition() - getPosition(), hexagonRadius);
		AxialCoordinates hexagon;
		int layers = 5;
		int x = offset.x + coordinates.q;
		int y = offset.y + coordinates.r;

		if (0 <= x && x < matrixLength && 0 <= y && y < matrixLength)
		{
			if (gridMatrix[x][y] == NULL)
			{
				Color color = gridMatrix[x][y]->getColor();
				int additive = 2 * (layers + 1);

				color.r = (color.r - additive >= 0) ? color.r - additive : 0;
				color.g = (color.g - additive >= 0) ? color.g - additive : 0;
				color.b = (color.b - additive >= 0) ? color.b - additive : 0;

				gridMatrix[x][y]->setColor(color);
			}
		}

		for (int k = 1; k <= layers; k++)
		{
			hexagon = AxialCoordinates(-k, k);

			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < k; j++)
				{
					int x = offset.x + hexagon.q + coordinates.q;
					int y = offset.y + hexagon.r + coordinates.r;

					if (0 <= x && x < matrixLength && 0 <= y && y < matrixLength)
					{
						if (gridMatrix[x][y] != NULL)
						{
							Color color = gridMatrix[x][y]->getColor();
							int additive = 2 * ((layers + 1) - k);

							color.r = (color.r - additive >= 0) ? color.r - additive : 0;
							color.g = (color.g - additive >= 0) ? color.g - additive : 0;
							color.b = (color.b - additive >= 0) ? color.b - additive : 0;

							gridMatrix[x][y]->setColor(color);
						}
					}
					hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
				}
			}
		}
	}

	// Beware this section can change if this territory is active.
	for (int i = 0; i < borderCoordinates.size(); i++)
	{
		FloorTile * tile = gridMatrix[offset.x + borderCoordinates[i].q][offset.y + borderCoordinates[i].r];

		if (Collision::hitBoxesOverlap(tile->getBoundingBox(), player->getBoundingBox()))
		{
			std::list<Vector2f> penetration;

			if (SAT::collides(player->getConvexHull(), tile->getConvexHull(), player->getSpeed(), penetration))
			{
				if (player->getIsInSafeZone())
				{
					Vector2f direction = Vector2fMath::unitVector(player->getPosition() - position);
					Vector2f seekPosition = position + (direction * radius * 1.5f);

					world->changeTerritory(seekPosition);

					break;
				}
				else
				{
					player->translate(SAT::getShortestPenetration(penetration));

					break;
				}
			}
		}
	}	
}