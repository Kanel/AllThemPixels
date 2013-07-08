#include "Player.h"
#include "Territory.h"
#include "UpdateInfo.h"
#include "World.h"
#include "PlayerCustomizationUI.h"
#include "VertexCluster.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

int main(int argc, char ** argv)
{
	int elapsedGameTime = 0;
	int updateInterval = 10;
	char * windowtitle = "All them Pixels!";
	bool fullscreen = true;
	Uint32 windowStyle = Style::Resize;	
	ContextSettings settings(0, 0, 8, 2, 0);
	VideoMode videoMode(1024, 1024);
    RenderWindow window(videoMode, windowtitle, 7, settings);
	World world(Vector2f(), 2048, 0, 1);
	Territory * t = world.getTerritory(AxialCoordinates(0, 0));
	Player * player;
	PlayerConfiguration playerconfig;
	PlayerCustomizationUI ui(Vector2f(0, 0));
	VertexCluster playerCluster;
	Time sleepDuration = milliseconds(updateInterval);
	UpdateInfo info;

	playerCluster.create(VertexCluster::HexagonSource);
	playerCluster.create(VertexCluster::RectangleSource);

	info.updateInterval = updateInterval;
	info.elapsedGameTime = 0;

	playerconfig.hp = 10000000;
	playerconfig.speed = 1.5;
	playerconfig.weaponConfig.cooldown = 100;
	playerconfig.weaponConfig.damage = 100;
	playerconfig.weaponConfig.piercing = 1;
	playerconfig.weaponConfig.speed = 10;
	playerconfig.weaponConfig.spread = 5;
	playerconfig.weaponConfig.ttl = 55;

	player = new Player(t->getSpawnQueue(), playerconfig, Vector2f(0, 0), playerCluster.getCollection(0));

	t->active = true;
	t->player = player;
	t->addEntity(player);

	while (world.isActive() && window.isOpen())
    {
		Event event;		

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		info.elapsedGameTime += updateInterval;

		world.update(info);
		window.setView(world.getView(window.getView()));
		ui.align(window.getView());

		if (ui.update(info) == PlayerCustomizationUI::Changed)
		{
			player->setConfiguration(ui.getConfiguration());
		}

		window.clear();		
		world.draw(&window);
		window.draw(playerCluster); // This contains the player and all its projectiles.
		window.draw(ui);
		window.display();
			
		// Sleep until the next frame needs to be rendered.
		sleep(sleepDuration);
    }

    return 0;
}