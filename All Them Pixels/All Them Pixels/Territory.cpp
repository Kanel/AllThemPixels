#include "Territory.h"

void Territory::colorTiles(HexagonGridStorage &storage, Color base)
{
	int tiles = HexagonGrid::getNumberOfTiles(layers);
	HexagonGrid grid(Hexagon::FlatTopped);

	for (int i = 0; i < tiles; i++)
	{
		int grayness = rand() % TERRITORY_FLOOR_TILES_GRAYNESS;
		AxialCoordinates coordinates = grid.next();
				
		storage[coordinates]->setColor(Color(base.r - grayness, base.g - grayness, base.b -grayness, base.a));
	}
}

void Territory::colorBorderTiles()
{
	for (int i = 0; i < borderCoordinates.size(); i++)
	{
		int redness = rand() % TERRITORY_BORDER_TILES_REDNESS;

		floorTiles[borderCoordinates[i].q][borderCoordinates[i].r]->setColor(Color(UCHAR_MAX - redness, 0, 0));
	}
}

void Territory::colorSafeZoneTiles()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < safeZonesTiles[i].size(); j++)
		{
			int greenness = rand() % TERRITORY_SAFE_TILES_GREENESS;

			safeZonesTiles[i][j]->setColor(Color(0, UCHAR_MAX - greenness, 0));
		}
	}
}

// Darkens the color of the floor tiles around enemies, the effect is cumulative. The
// effect fades as the distance between the enemy and floor tiles increses.
void Territory::colorEnemyAuraTiles()
{
	Color color = TERRITORY_ENEMY_AURA_COLOR;
	
	for (auto enemy : enemies)
	{
		HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);
		AxialCoordinates origin = grid.getAxialCoordinates(enemy->getPosition() - getPosition());
		int layers = TERRITORY_ENEMY_AURA_SIZE;
		int tiles = grid.getNumberOfTiles(layers);

		grid.setOrigin(origin);

		for (int i = 0; i < tiles; i++)
		{
			int k;
			AxialCoordinates coordinates = grid.next(k);
			
			if (floorTiles.contains(coordinates))
			{
				Color tileColor = floorTiles[coordinates]->getColor();
				int amount = TERRITORY_ENEMY_AURA_INTENSITY * (layers - k);
				int r = tileColor.r + sign(color.r - tileColor.r) * amount;
				int g = tileColor.g + sign(color.g - tileColor.g) * amount;
				int b = tileColor.b + sign(color.b - tileColor.b) * amount;
				int a = tileColor.a;
					
				r = limit(r, 0, UCHAR_MAX);
				g = limit(g, 0, UCHAR_MAX);
				b = limit(b, 0, UCHAR_MAX);

				floorTiles[coordinates]->pushColor(Color(r, g, b, a));
			}			
		}		
	}
}

// Remove effects such as enemy aura from floor tiles.
void Territory::cleanFloorTiles()
{
	int tiles = HexagonGrid::getNumberOfTiles(layers);
	HexagonGrid grid(Hexagon::FlatTopped);

	for (int i = 0; i < tiles; i++)
	{
		floorTiles[grid.next()]->resetColor();
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
	int layers = TERRITORY_SAFE_SIZE;
	int tiles = HexagonGrid::getNumberOfTiles(layers);

	for(int direction = 0; direction < 6; direction++)
	{
		HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);

		grid.setOrigin(origins[direction][0], origins[direction][1]);

		for (int i = 0; i < tiles; i++)
		{
			AxialCoordinates coordinate = grid.next();

			if (floorTiles.contains(coordinate))
			{
				safeZonesTiles[direction].push_back(floorTiles[coordinate]);
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

		if (!projectile->isExpended())
		{
			for (auto enemy : enemies)
			{
				if (Collision::isClose(enemy, projectile))
				{
					if (!projectile->hasPierced(enemy))
					{
						// Todo: play sounds
						enemy->modHP(-projectile->getDamage());						
						projectile->addPiercedTarget(enemy);

						if (projectile->isExpended())
						{
							effects.push_back(new ParticleSystem(info.elapsedGameTime, 100, projectile->getPosition(), projectile->getSpeed(), entityCluster.getCollection(2)));
						
							break;
						}
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
			if (!projectile->hasPierced(player))
			{
				Vector2f direction = Vector2fMath::unitVector(projectile->getPosition() - player->getPosition());

				sounds->play(SoundTypes::PlayerHit, player->getPosition() + direction * 20.0f);
				player->modHP(-projectile->getDamage());
				player->fade();

				projectile->addPiercedTarget(player);

				if (projectile->isExpended())
				{
					effects.push_back(new ParticleSystem(info.elapsedGameTime, 100, projectile->getPosition(), projectile->getSpeed(), entityCluster.getCollection(2)));
						
					continue;
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
			Hexagon * tile = safeZonesTiles[i][j];

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
		Hexagon * tile = floorTiles[borderCoordinates[i].q][borderCoordinates[i].r];

		if (Collision::hitBoxesOverlap(tile->getBoundingBox(), player->getBoundingBox()))
		{
			std::list<Vector2f> penetration;

			if (SAT::collides(player->getConvexHull(), tile->getConvexHull(), player->getSpeed(), penetration))
			{
				// Move the player into another territory if the player is in a safe zone.
				if (player->getIsInSafeZone())
				{
					Vector2f direction = Vector2fMath::unitVector(player->getPosition() - position);
					Vector2f seekPosition = position + (direction * radius * 1.1f);

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

void Territory::prepareProbes(Vector2f center)
{
	float angle = 0;
	float angleStep;
	HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);

	probes.resize(TERRITORY_PROBE_COUNT);

	angleStep = (2 * M_PI) / probes.size();

	for (int i = 0; i < probes.size(); i++, angle += angleStep)
	{
		probes[i].speed = 5 + i % 3;
		probes[i].position = center + Vector2f(cosf(angle), sinf(angle)) * 1024.0f;
		probes[i].direction = Vector2fMath::unitVector(center - probes[i].position);
	}
}

void Territory::updateProbes(Color color, Vector2f center)
{	
	for (int i = 0; i < probes.size(); i++)
	{
		probes[i].position += probes[i].direction * probes[i].speed;
		probes[i].direction = Vector2fMath::unitVector(center - probes[i].position);
	}

	for (auto probe : probes)
	{
		int layers = TERRITORY_PROBE_SIZE;
		int tiles = HexagonGrid::getNumberOfTiles(layers);
		HexagonGrid grid(Hexagon::FlatTopped, hexagonRadius);
		AxialCoordinates origin = grid.getAxialCoordinates(probe.position - position);
		
		grid.setOrigin(origin);

		for (int i = 0; i < tiles; i++)
		{
			int k;
			AxialCoordinates coordinates = grid.next(k);

			if (fadeTiles.contains(coordinates))
			{
				Color tileColor = fadeTiles[coordinates]->getColor();
				int amount = TERRITORY_PROBE_INTENSITY * (layers - k);
				int r = tileColor.r + sign(color.r - tileColor.r) * amount;
				int g = tileColor.g + sign(color.g - tileColor.g) * amount;
				int b = tileColor.b + sign(color.b - tileColor.b) * amount;
				int a = tileColor.a + sign(color.a - tileColor.a) * amount;

				r = limit(r, 0, UCHAR_MAX);
				g = limit(g, 0, UCHAR_MAX);
				b = limit(b, 0, UCHAR_MAX);
				a = limit(a, 0, UCHAR_MAX);

				fadeTiles[coordinates]->setColor(Color(r, g, b, a));
			}
		}		
	}
}

Territory::Territory(Vector2f position, float radius, World * world, SpawnConfiguration spawnConfig) : enemySpawner(spawnConfig)
{
	// Flat topped hexagons
	float hexagonWidth;
	float hexagonHeight;
	int numberOfLayersHorizontal;
	int numberOfLayersVertical;	
	int spawnRingSize = TERRITORY_ENEMY_SPAWN_RING_SIZE;
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
	fadeTiles = NULL;

	// Calculate how many layers will fit the radius.
	hexagonWidth = hexagonRadius * 2;
	hexagonHeight = sqrt(3)/2 * hexagonWidth;
	numberOfLayersHorizontal = ((2 * radius) - hexagonWidth) / (hexagonWidth * 1.5f);
	numberOfLayersVertical = ((2 * radius) - hexagonHeight) / (2 * hexagonHeight);
	layers = (numberOfLayersHorizontal < numberOfLayersVertical) ? numberOfLayersHorizontal : numberOfLayersVertical;

	// Prepare bounding box.
	boundingBox.width = (layers * hexagonWidth * 1.5f) + hexagonWidth;
	boundingBox.height = ((layers * 2) + 1) * hexagonHeight;
	boundingBox.left = position.x - (boundingBox.width / 2);
	boundingBox.top = position.y - (boundingBox.height / 2);

	// Prepare vertex collections.
	tileCluster.create(8, grid.getNumberOfTiles(layers), 1, PrimitiveType::TrianglesStrip);
	entityCluster.create(VertexCluster::HexagonSource);
	entityCluster.create(VertexCluster::HexagonSource);
	entityCluster.create(VertexCluster::RectangleSource);
	fadeTileCluster.create(8, grid.getNumberOfTiles(layers), 1, PrimitiveType::TrianglesStrip);

	// Prepare the background tiles.
	floorTiles = grid.generateGrid(position, layers, tileCluster.getCollection(0));
	
	// Prepare the win/loss tiles.
	fadeTiles = grid.generateGrid(position, layers, fadeTileCluster.getCollection(0));

	// Prepare border.
	borderCoordinates = grid.getRingCoordinates(layers);

	// Preapare the spawn grid.
	spawnGrid = grid.getRingCoordinates(spawnRingSize);

	// Prepare safe zones.
	prepareSafeZoneTiles(layers);
	spatialPartitionSafeRoomTiles();

	// Color tiles.
	colorTiles(floorTiles, TERRITORY_FLOOR_TILES_BASE_COLOR);
	colorTiles(fadeTiles, TERRITORY_FADE_TILES_BASE_COLOR);
	colorBorderTiles();
	colorSafeZoneTiles();
}

Territory::~Territory()
{
	int tiles = HexagonGrid::getNumberOfTiles(layers);
	HexagonGrid grid(Hexagon::FlatTopped);

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

	for (int i = 0; i < tiles; i++)
	{
		AxialCoordinates coordinates = grid.next();

		delete floorTiles[coordinates.q][coordinates.r];
	}
	grid.reset();

	for (int i = 0; i < tiles; i++)
	{
		AxialCoordinates coordinates = grid.next();

		delete floorTiles[coordinates.q][coordinates.r];
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
				this->sounds->play(SoundTypes::Shot, entity->getPosition());
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
			assert(false && "That unit type cannot be added!");
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
		if (floorTiles.contains(coordinate))
		{
			spawnPoints.push_back(position + grid.getPosition(coordinate));
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

void Territory::fade(Color color, Vector2f center)
{
	if (probes.size() == 0)
	{
		prepareProbes(center);
	}
	else
	{
		updateProbes(color, center);
	}
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
		active = false;
		player = NULL;
	}
}

void Territory::draw(RenderTarget& target, RenderStates states) const
{	
	target.draw(tileCluster);
	target.draw(entityCluster);

	if (player != NULL)
	{
		target.draw(*player);
	}
	if (probes.size() > 0)
	{
		target.draw(fadeTileCluster);
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