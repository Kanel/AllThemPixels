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
	int tiles = HexagonGrid::getNumberOfTiles(layers);
	HexagonGrid grid(Hexagon::PointyTopped, territoryRadius);
	SpawnConfiguration spawnConfig;
	PlayerConfiguration playerconfig;

	this->territoryRadius = territoryRadius;
	this->territorySpacing = territorySpacing;
	this->layers = layers;
	this->player = NULL;

	playerconfig.hp = PLAYER_DEFAULT_HP;
	playerconfig.speed = PLAYER_DEFAULT_SPEED;
	playerconfig.weaponConfig.cooldown = PLAYER_DEFAULT_WEAPON_COOLDOWN;
	playerconfig.weaponConfig.damage = PLAYER_DEFAULT_WEAPON_DAMAGE;
	playerconfig.weaponConfig.piercing = PLAYER_DEFAULT_WEAPON_PIERCING;
	playerconfig.weaponConfig.speed = PLAYER_DEFAULT_WEAPON_SPEED;
	playerconfig.weaponConfig.spread = PLAYER_DEFAULT_WEAPON_SPREAD;
	playerconfig.weaponConfig.ttl = PLAYER_DEFAULT_WEAPON_TTL;

	player = new Player(NULL, playerconfig, Vector2f(0, 0));
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

	for (int i = 0; i < tiles; i++)
	{
		Vector2f territoryPosition;
		AxialCoordinates coordinates = grid.next(territoryPosition);

		territories[offset.x + coordinates.q][offset.y + coordinates.r] = new Territory(position + territoryPosition, territoryRadius, this, spawnConfig);
	}

	//randomize starting territory
	grid.getRingCoordinates(layers);

	map = new Map(Vector2f(), layers, 50, Hexagon::Style::FlatTopped);

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
	HexagonGrid grid(Hexagon::PointyTopped, territoryRadius);
	AxialCoordinates nextCoordinates = grid.getAxialCoordinates(position);
	Territory * current = getTerritory(currentTerritoryCoordinates);
	Territory * next = getTerritory(nextCoordinates);
	Player * player = current->player;
	
	current->deactivate();
	next->activate(player);

	player->setPosition(position);//next->getPosition());

	map->setPlayerLocation(nextCoordinates);
	if (current->isCleared()) map->addClearedTerritory(currentTerritoryCoordinates);

	currentTerritoryCoordinates = nextCoordinates;
}

void World::activate(AxialCoordinates coordinates)
{
	Territory * territory = getTerritory(coordinates);

	currentTerritoryCoordinates = coordinates;

	map->setPlayerLocation(currentTerritoryCoordinates);

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

Player * World::getPlayer()
{
	return player;
}

void World::draw(RenderTarget& target, RenderStates states) const
{
	int tiles = HexagonGrid::getNumberOfTiles(layers);
	HexagonGrid grid(Hexagon::FlatTopped);

	for (int i = 0; i < tiles; i++)
	{
		AxialCoordinates coordinates = grid.next();

		if (Collision::isWithinWindow(territories[coordinates.q][coordinates.r]->getBoundingBox(), target.getView()))
		{
			target.draw(*(territories[coordinates.q][coordinates.r]));
		}
	}
	if (Joystick::isButtonPressed(0,6)) //is this the back button?
	{
		target.draw(*map);

	}
}

void World::update(UpdateInfo info, Sounds * sounds)
{
	if (Joystick::isButtonPressed(0, GAMEPAD_BACK))
	{
		//paused = true;
		map->setPosition(player->getPosition());// plus some offset? MAYBE!
	}
	else
	{
		int tiles = HexagonGrid::getNumberOfTiles(layers);
		HexagonGrid grid(Hexagon::FlatTopped);

		for (int i = 0; i < tiles; i++)
		{
			AxialCoordinates coordinates = grid.next();

			if (territories[coordinates.q][coordinates.r]->isActive())
			{
				territories[coordinates.q][coordinates.r]->integrateSpawnQueue();
				territories[coordinates.q][coordinates.r]->update(info, sounds);
				territories[coordinates.q][coordinates.r]->cleanup();
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