#include "Territory.h"

void Territory::colorFloorTiles()
{
	for (int i = 0; i < floorTilesLength; i++)
	{
		for (int j = 0; j < floorTilesLength; j++)
		{
			if (floorTiles[i][j] != NULL)
			{
				int grayness = rand() % 50;

				floorTiles[i][j]->setColor(Color(255 - grayness, 255 - grayness, 255 -grayness));
			}
		}
	}
}

void Territory::colorBorderTiles()
{
	for (int i = 0; i < borderCoordinates.size(); i++)
	{
		int redness = rand() % 50;

		floorTiles[offset.x + borderCoordinates[i].q][offset.y + borderCoordinates[i].r]->setColor(Color(255 - redness, 0, 0));
	}
}

void Territory::colorSafeZoneTiles()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < safeZonesTiles[i].size(); j++)
		{
			int greenness = rand() % 50;

			safeZonesTiles[i][j]->setColor(Color(0, 255 - greenness, 0));
		}
	}
}

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

		safeZonesTiles[direction].push_back(floorTiles[x][y]);

		for (int k = 1; k <= layers; k++)
		{
			hexagon = AxialCoordinates(origins[direction][0] + -k, origins[direction][1] + k);

			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < k; j++)
				{
					x = offset.x + hexagon.q;
					y = offset.y + hexagon.r;

					if (0 <= x && x < floorTilesLength && 0 <= y && y < floorTilesLength)
					{
						if (floorTiles[x][y] != NULL)
						{
							safeZonesTiles[direction].push_back(floorTiles[x][y]);
						}
					}
					hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
				}
			}
		}
	}
}

void Territory::prepareAI()
{
	aiProperties[0] = AI::generate((rand() % 5) + 1);
	aiProperties[1] = AI::generate((rand() % 5) + 1);
	aiProperties[2] = AI::generate((rand() % 5) + 1);
	aiProperties[3] = AI::generate((rand() % 5) + 1);
}

void Territory::prepareWeapons()
{
	WeaponConfiguration config;

	config.cooldown = 500;
	config.damage = 20;
	config.piercing = 1;
	config.speed = 8;
	config.spread = 4;
	config.ttl = 500;

	enemyWeapons[0] = Weapon(config, entityCluster.getCollection(0));
	enemyWeapons[1] = Weapon(config, entityCluster.getCollection(0));
	enemyWeapons[2] = Weapon(config, entityCluster.getCollection(0));
	enemyWeapons[3] = Weapon(config, entityCluster.getCollection(0));
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
	float hexagonWidth;
	float hexagonHeight;
	int numberOfLayersHorizontal;
	int numberOfLayersVertical;
	int layers;
	int spawnRingSize = 47;
	HexagonGrid grid(Hexagon::FlatTopped);
	WeaponConfiguration wc;

	// Some variables.
	this->hexagonRadius = TERRITORY_TILE_SIZE;
	this->position = position;
	this->radius = radius;
	this->world = world;
	this->sounds = NULL;

	active = false;	
	player = NULL;

	// Calculate how many layers will fit the radius.
	hexagonWidth = hexagonRadius * 2;
	hexagonHeight = sqrt(3)/2 * hexagonWidth;
	numberOfLayersHorizontal = ((2 * radius) - hexagonWidth) / (hexagonWidth * 1.5f);
	numberOfLayersVertical = ((2 * radius) - hexagonHeight) / (2 * hexagonHeight);
	layers = (numberOfLayersHorizontal < numberOfLayersVertical) ? numberOfLayersHorizontal : numberOfLayersVertical;

	// Floor tile matrix grid
	offset.x = layers;
	offset.y = layers;
	floorTilesLength = (layers * 2) + 1;

	// Prepare bounding box.
	boundingBox.width = (layers * hexagonWidth * 1.5f) + hexagonWidth;
	boundingBox.height = ((layers * 2) + 1) * hexagonHeight;
	boundingBox.left = position.x - (boundingBox.width / 2);
	boundingBox.top = position.y - (boundingBox.height / 2);

	// Prepare vertex collections.
	tileCluster.create(8, grid.getNumberOfTiles(layers), 1, PrimitiveType::TrianglesStrip);
	entityCluster.create(VertexCluster::HexagonSource);
	entityCluster.create(VertexCluster::RectangleSource);

	// Prepare the floor tiles.
	floorTiles = FloorTile::generateGrid(position, hexagonRadius, layers, tileCluster.getCollection(0));
	
	// Prepare border.
	borderCoordinates = grid.getRingCoordinates(layers);

	// Preapare the spawn grid.
	spawnGrid = grid.getRingCoordinates(spawnRingSize);

	// Prepare safe zones.
	prepareSafeZoneTiles(layers);
	spatialPartitionSafeRoomTiles();
	
	// Prepare weapon & AI.
	prepareAI();
	prepareWeapons();

	// Color tiles.
	colorFloorTiles();
	colorBorderTiles();
	colorSafeZoneTiles();

}

Territory::~Territory()
{
	integrateSpawnQueue();

	for (auto projectile : enemyProjectiles)
	{
		delete projectile;		
	}

	for (auto projectile : playerProjectile)
	{
		delete projectile;		
	}

	for (auto projectile : enemies)
	{
		delete projectile;		
	}

	for (int i = 0; i < floorTilesLength; i++)
	{
		for (int j = 0; j < floorTilesLength; j++)
		{
			if (floorTiles[i][j] != NULL)
			{
				delete floorTiles[i][j];
			}
		}
	}
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
				delete *(it);

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
				delete *(it);

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
				delete *(it);

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
			if (!this->sounds) 
			{
				this->sounds->play(SoundTypes::Shot);
			}
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

		if (0 <= x && x < floorTilesLength && 0 <= y && y < floorTilesLength)
		{
			if (floorTiles[x][y] != NULL)
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

void Territory::update(UpdateInfo info, Sounds * sounds)
{
	this->sounds = sounds;

	// Update all projectiles projectiles
	for (auto projectile : playerProjectile)
	{
		projectile->update(info);

		for (auto enemy : enemies)
		{
			if (Collision::isClose(enemy, projectile))
			{					
				enemy->modHP(-projectile->getDamage());

				if (!projectile->hasPierced(enemy))
				{
					if (projectile->getPiercing() > 1)
					{
						projectile->addPiercedTarget(enemy);
					}
					else
					{
						projectile->expend();
					}
				}

				if (enemy->isExpended())
				{
					sounds->play(SoundTypes::EnemyDeath, enemy->getPosition());
					player->addSkillPoints(enemy->getSkillPoints());
				}
			}
		}		
	}

	// Update all enemies.
	if (!player->getIsInSafeZone())
	{
		for (auto enemy : enemies)
		{
			AI::update(&spawnQueue, enemy, player, info);
		}
	}

	// Check if the player is hit by enemy projectiles.
	for (auto projectile : enemyProjectiles)
	{
		projectile->update(info);

		if (Collision::isClose(player, projectile))
		{
			sounds->play(SoundTypes::PlayerHit);
			player->modHP(-projectile->getDamage());
			player->fade();
			projectile->expend();

			if (!projectile->hasPierced(player))
			{
				if (projectile->getPiercing() > 1)
				{
					projectile->addPiercedTarget(player);
				}
				else
				{
					projectile->expend();
				}
			}
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

	// Spawn enemy
	Enemy * enemy = new Enemy(100, getSpawnLocation(), entityCluster.getCollection(1));

	enemy->educate(aiProperties[0]);
	enemy->arm(enemyWeapons[rand() % 4]);
	addEntity(enemy);
	
	// Background tile stuff.
	HexagonGrid grid(Hexagon::FlatTopped);

	for (int i = 0; i < floorTilesLength; i++)
	{
		for (int j = 0; j < floorTilesLength; j++)
		{
			if (floorTiles[i][j] != NULL)
			{
				floorTiles[i][j]->resetColor();
			}
		}
	}

	// Enemy aura colorization on floor tiles. 
	for (auto enemy : enemies)
	{
		AxialCoordinates coordinates = grid.getAxialCoordinates(enemy->getPosition() - getPosition(), hexagonRadius);
		AxialCoordinates hexagon;
		int layers = 5;
		int x = offset.x + coordinates.q;
		int y = offset.y + coordinates.r;

		// Core tile
		if (0 <= x && x < floorTilesLength && 0 <= y && y < floorTilesLength)
		{
			if (floorTiles[x][y] != NULL)
			{
				Color color = floorTiles[x][y]->getColor();
				int additive = 2 * (layers + 1);

				color.r = (color.r - additive >= 0) ? color.r - additive : 0;
				color.g = (color.g - additive >= 0) ? color.g - additive : 0;
				color.b = (color.b - additive >= 0) ? color.b - additive : 0;

				floorTiles[x][y]->pushColor(color);
			}
		}

		// Layer tiles
		for (int k = 1; k <= layers; k++)
		{
			hexagon = AxialCoordinates(-k, k);

			for (int i = 0; i < 6; i++)
			{
				for (int j = 0; j < k; j++)
				{
					int x = offset.x + hexagon.q + coordinates.q;
					int y = offset.y + hexagon.r + coordinates.r;

					if (0 <= x && x < floorTilesLength && 0 <= y && y < floorTilesLength)
					{
						if (floorTiles[x][y] != NULL)
						{
							Color color = floorTiles[x][y]->getColor();
							int additive = 2 * ((layers + 1) - k);

							color.r = (color.r - additive >= 0) ? color.r - additive : 0;
							color.g = (color.g - additive >= 0) ? color.g - additive : 0;
							color.b = (color.b - additive >= 0) ? color.b - additive : 0;

							floorTiles[x][y]->pushColor(color);
						}
					}
					hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
				}
			}
		}
	}

	// Beware this section can change if this territory is active.
	// Handle the case of the player trying to cross the territory border.
	for (int i = 0; i < borderCoordinates.size(); i++)
	{
		FloorTile * tile = floorTiles[offset.x + borderCoordinates[i].q][offset.y + borderCoordinates[i].r];

		if (Collision::hitBoxesOverlap(tile->getBoundingBox(), player->getBoundingBox()))
		{
			std::list<Vector2f> penetration;

			if (SAT::collides(player->getConvexHull(), tile->getConvexHull(), player->getSpeed(), penetration))
			{
				// Move the player into another territory if the player is in a safe zone.
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