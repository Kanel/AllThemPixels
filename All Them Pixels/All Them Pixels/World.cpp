#include "World.h"

vector<AIProperties> World::getAIProperties()
{
	vector<AIProperties> aiProperties(4);

	aiProperties[0] = AI::generate((rand() % 5) + 1);
	aiProperties[1] = AI::generate((rand() % 5) + 1);
	aiProperties[2] = AI::generate((rand() % 5) + 1);
	aiProperties[3] = AI::generate((rand() % 5) + 1);

	return aiProperties;
}

vector<WeaponConfiguration> World::getWeaponConfigurations()
{
	vector<WeaponConfiguration> enemyWeapons(3);
	WeaponConfiguration weaponA;
	WeaponConfiguration weaponB;
	WeaponConfiguration weaponC;

	weaponA.cooldown = 500;
	weaponA.damage = 20;
	weaponA.piercing = 1;
	weaponA.speed = 8;
	weaponA.spread = 4;
	weaponA.ttl = 500;

	weaponB.cooldown = 100;
	weaponB.damage = 4;
	weaponB.piercing = 1;
	weaponB.speed = 8;
	weaponB.spread = 4;
	weaponB.ttl = 500;

	weaponC.cooldown = 1000;
	weaponC.damage = 5000;
	weaponC.piercing = 1;
	weaponC.speed = 16;
	weaponC.spread = 4;
	weaponC.ttl = 500;

	enemyWeapons[0] = weaponA;
	enemyWeapons[1] = weaponB;
	enemyWeapons[2] = weaponC;

	return enemyWeapons;
}

World::World(Vector2f position, float territoryRadius, float territorySpacing, int layers)
{
	HexagonGrid grid(Hexagon::PointyTopped);
	SpawnConfiguration spawnConfig;
	PlayerConfiguration playerconfig;

	playerCluster.create(VertexCluster::HexagonSource);
	playerCluster.create(VertexCluster::RectangleSource);

	this->territoryRadius = territoryRadius;
	this->territorySpacing = territorySpacing;
	this->layers = layers;
	this->player = NULL;

	playerconfig.hp = 1000000;
	playerconfig.speed = 1.5;
	playerconfig.weaponConfig.cooldown = 100;
	playerconfig.weaponConfig.damage = 100;
	playerconfig.weaponConfig.piercing = 1;
	playerconfig.weaponConfig.speed = 20;
	playerconfig.weaponConfig.spread = 5;
	playerconfig.weaponConfig.ttl = 55;

	player = new Player(NULL, playerconfig, Vector2f(0, 0), playerCluster.getCollection(0));
	tileSize = territoryRadius + (territorySpacing / 2);
	matrixSize = (layers * 2) + 1;
	offset = Vector2i(layers, layers);

	spawnConfig.spawnPoints = 10000;
	spawnConfig.spawnRate = 500;
	spawnConfig.aiProperties = getAIProperties();
	spawnConfig.enemyWeapons = getWeaponConfigurations();

	territories = new Territory**[matrixSize];

	// Territory matrix
	for (int i = 0; i < matrixSize; i++)
	{
		territories[i] = new Territory*[matrixSize];

		for (int j = 0; j < matrixSize; j++)
		{
			territories[i][j] = NULL;
		}
	}

	territoryCount = 0;

	// Center territory
	territories[offset.x][offset.y] = new Territory(position, territoryRadius, this, spawnConfig);

	// Layer territories
	for (int k = 1; k <= layers; k++)
	{
		AxialCoordinates hexagon(-k, k);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < k; j++)
			{
				territories[offset.x + hexagon.q][offset.y + hexagon.r] = new Territory(position + grid.getPosition(hexagon, tileSize), territoryRadius, this, spawnConfig);

				hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
			}
		}
	}
}

Territory * World::getCurrentTerritory()
{
	return getTerritory(currentTerritoryCoordinates);
}

Territory * World::getTerritory(AxialCoordinates coordinates)
{
	return territories[offset.x + coordinates.q][offset.y + coordinates.r];
}

void World::changeTerritory(Vector2f position)
{
	HexagonGrid grid(Hexagon::PointyTopped);
	AxialCoordinates nextCoordinates = grid.getAxialCoordinates(position, tileSize);
	Territory * current = getTerritory(currentTerritoryCoordinates);
	Territory * next = getTerritory(nextCoordinates);
	Player * player = current->player;
	
	current->deactivate();
	next->activate(player);

	player->setPosition(next->getPosition());

	currentTerritoryCoordinates = nextCoordinates;
}

void World::activate(AxialCoordinates coordinates)
{
	Territory * territory = getTerritory(coordinates);

	currentTerritoryCoordinates = coordinates;

	territory->activate(player);

	player->setPosition(territory->getPosition());
}

bool World::isActive()
{
	return getTerritory(currentTerritoryCoordinates)->isActive();
}

bool World::isCleared()
{
	Territory * territory = getTerritory(AxialCoordinates());

	return territory->isCleared();
}

void World::draw(RenderWindow * window)
{
	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (territories[i][j] != NULL)
			{
				if (Collision::isWithinWindow(territories[i][j]->getBoundingBox(), window->getView()))
				{
					territories[i][j]->draw(window);
				}
			}
		}
	}
	window->draw(playerCluster);
}

void World::update(UpdateInfo info, Sounds * sounds)
{
	for (int i = 0; i < matrixSize; i++)
	{
		for (int j = 0; j < matrixSize; j++)
		{
			if (territories[i][j] != NULL && territories[i][j]->isActive())
			{
				territories[i][j]->integrateSpawnQueue();
				territories[i][j]->update(info, sounds);
				territories[i][j]->cleanup();
			}
		}
	}
}

View World::getView(View reference)
{
	Territory * current = getTerritory(currentTerritoryCoordinates);

	if (current->isActive())
	{
		reference.setCenter(current->player->getPosition());
	}
	return reference;
}