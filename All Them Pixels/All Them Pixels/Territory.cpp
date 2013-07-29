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

// Darkens the color of the floor tiles around enemies, the effect is cumulative. The
// effect fades as the distance between the enemy and floor tiles increses.
void Territory::colorEnemyAuraTiles()
{
	HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);

	for (auto enemy : enemies)
	{
		AxialCoordinates coordinates = grid.getAxialCoordinates(enemy->getPosition() - getPosition());
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
}

// Remove effects such as enemy aura from floor tiles.
void Territory::cleanFloorTiles()
{
	HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);

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
}

// Determines which floor tiles belong to the safe zones. There are 6 safe zones.
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
	HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);

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

// Spatialy partition the safe zone floor tiles to speed up collision detections.
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

// Updates player projectiles and checks if they are colliding with any enemies.
void Territory::updatePlayerProjectiles(UpdateInfo info)
{
	for (auto projectile : playerProjectiles)
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
						effects.push_back(new ParticleSystem(info.elapsedGameTime, 100, projectile->getPosition(), entityCluster.getCollection(1)));
					}
				}
			}
		}		
	}
}

// Updates enemy projectiles and checks collisions against the player and safe zones.
void Territory::updateEnemyProjectiles(UpdateInfo info)
{
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
					effects.push_back(new ParticleSystem(info.elapsedGameTime, 100, projectile->getPosition(), entityCluster.getCollection(1)));
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
}

void Territory::updateEnemies(UpdateInfo info)
{
	if (!player->getIsInSafeZone())
	{
		for (auto enemy : enemies)
		{
			AI::update(&spawnQueue, enemy, player, info);
		}
	}
}

// Updates the player and checks if the player is in a safe zone.
void Territory::updatePlayer(UpdateInfo info)
{
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
}

void Territory::updateEffects(UpdateInfo info)
{
	for (auto effect : effects)
	{
		effect->update(info);
	}
}

// Can alter if this territory is active.
void Territory::updateBorderTiles()
{
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
					Vector2f seekPosition = position + (direction * radius * 1.01f);

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


void Territory::borderControl()
{
	Vector2f center = position;

	Vector2f playerCenter = player->getPosition();
	playerCenter.x += 10; //fix
	playerCenter.y += 10; //fix

	int quadrant;

	if (playerCenter.x < center.x)
		if (playerCenter.y < center.y)
			quadrant = 4;
		else
			quadrant = 3;
	else
		if (playerCenter.y < center.y)
			quadrant = 1;
		else
			quadrant = 2;

	//imagine player position in quadrant four.
	switch (quadrant)
	{
	case 1:
		playerCenter.x = playerCenter.x - 2*(playerCenter.x - center.x);
		break;
	case 2:
		playerCenter.x = playerCenter.x - 2*(playerCenter.x - center.x);
		playerCenter.y = playerCenter.y - 2*(playerCenter.y - center.y);
		break;
	case 3:
		playerCenter.y = playerCenter.y - 2*(playerCenter.y - center.y);
		break;
	case 4:
		break;
	default:
		break;
	}
	/*
		Vector2f a = corner position;
		Vector2f p = player center;
		Vector2f n = unit vector pointing to next corner or down;
		Vector2f v1 = Vector2fMath::dot((a-p), n)*n;

		float distance = Vector2fMath::length((a-p)-v1);
		if distance is less than some radius of player:
			player->translate
		if in safe and distance is smaller.
			change territory
	*/
}

Territory::Territory(Vector2f position, float radius, World * world, SpawnConfiguration spawnConfig) : enemySpawner(spawnConfig)
{
	// Flat topped hexagons
	float hexagonWidth;
	float hexagonHeight;
	int numberOfLayersHorizontal;
	int numberOfLayersVertical;
	int layers;
	int spawnRingSize = 47;
	HexagonGrid grid(Hexagon::FlatTopped, TERRITORY_TILE_SIZE);
	WeaponConfiguration wc;

	// Some variables.
	this->hexagonRadius = TERRITORY_TILE_SIZE; // If changed change the parameter to grid!
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

	for (auto projectile : playerProjectiles)
	{
		delete projectile;		
	}

	for (auto projectile : enemies)
	{
		delete projectile;		
	}

	for (auto effect : effects)
	{
		delete effect;		
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
		delete[] floorTiles[i];
	}
	delete[] floorTiles;
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
		for (std::list<Projectile *>::iterator it = playerProjectiles.begin(); it != playerProjectiles.end();)
		{
			if ((*it) == entity)
			{
				delete *(it);

				playerProjectiles.erase(it);

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
			playerProjectiles.push_back((Projectile *)entity);
		}
		else if(Flag::isFlagSet(entity->getType(), EnemyEntity))
		{
			enemies.push_back((Enemy *)entity);
		}
		else
		{
			// error
		}

		spawnQueue.pop();
	}
}

vector<Vector2f> Territory::getSpawnPoints()
{
	bool found = false;	
	HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);
	AxialCoordinates origin = grid.getAxialCoordinates(player->getPosition() - position);
	vector<Vector2f> spawnPoints;

	for (auto coordinate : spawnGrid)
	{
		int x = offset.x + coordinate.q;
		int y = offset.y + coordinate.r;

		if (0 <= x && x < floorTilesLength && 0 <= y && y < floorTilesLength)
		{
			if (floorTiles[x][y] != NULL)
			{
				spawnPoints.push_back(grid.getPosition(coordinate));
			}
		}
	}
	return spawnPoints;
}

void Territory::activate(Player * player)
{
	this->active = true;
	this->player = player;
	this->player->spawnQueue = &spawnQueue;
}

void Territory::deactivate()
{
	active = false;	
	player->spawnQueue = NULL;
	player = NULL;
}

bool Territory::isActive()
{
	return active;
}

bool Territory::isCleared()
{
	return enemies.size() == 0 && !enemySpawner.canSpawn();
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

	for (std::list<Projectile *>::iterator it = playerProjectiles.begin(); it != playerProjectiles.end();)
	{
		Projectile * entity = *it;

		if (entity->isExpended())
		{
			it = playerProjectiles.erase(it);

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
			sounds->play(SoundTypes::EnemyDeath, entity->getPosition());
			player->addSkillPoints(entity->getSkillPoints());

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

void Territory::draw(RenderTarget& target, RenderStates states) const
{	
	target.draw(tileCluster);
	target.draw(entityCluster);

	if (active)
	{
		target.draw(*player);
	}
}

void Territory::update(UpdateInfo info, Sounds * sounds)
{
	this->sounds = sounds;

	// Update all player projectiles
	updatePlayerProjectiles(info);

	// Update all enemies.
	updateEnemies(info);

	// Check if the player is hit by enemy projectiles.
	updateEnemyProjectiles(info);

	// Update player and safe zone checks.
	updatePlayer(info);

	// Update all effects.
	updateEffects(info);

	// Spawn enemy
	enemySpawner.update(getSpawnPoints(), spawnQueue, entityCluster.getCollection(1), entityCluster.getCollection(0), info);

	// Background tile stuff.
	cleanFloorTiles();

	// Enemy aura colorization on floor tiles.
	colorEnemyAuraTiles();

	// Beware this section can change if this territory is active.
	// Handle the case of the player trying to cross the territory border.
	updateBorderTiles();	
}