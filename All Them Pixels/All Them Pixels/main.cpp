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
	/*Clock c;
	Clock elapsedTime;*/
	int fps = 0;
	Territory * t = world.getTerritory(AxialCoordinates(0, 0));
	Player * player;
	PlayerConfiguration playerconfig;
	PlayerCustomizationUI ui(Vector2f(0, 0));
	VertexCluster cluster;
	Time sleepDuration = milliseconds(updateInterval);

	playerconfig.hp = 10000000;
	playerconfig.speed = 1.5;
	playerconfig.weaponConfig.cooldown = 100;
	playerconfig.weaponConfig.damage = 100;
	playerconfig.weaponConfig.piercing = 1;
	playerconfig.weaponConfig.speed = 10;
	playerconfig.weaponConfig.spread = 5;
	playerconfig.weaponConfig.ttl = 55;

	player = new Player(t->getSpawnQueue(), playerconfig, Vector2f(0, 0), cluster[VertexCluster::HexagonSource]);
	VertexCollection * col;

	t->active = true;
	t->player = player;
	t->addEntity(player);

	while (world.isActive())
    {
		/*if (c.getElapsedTime().asMilliseconds() >= updateInterval)
		{*/
			Event event;
			UpdateInfo info;
			Vector2f inGamePosition;

			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
				}
			}

			elapsedGameTime += updateInterval;

			info.updateInterval = updateInterval;
			info.elapsedGameTime = elapsedGameTime;

			world.update(info);
			window.setView(world.getView(window.getView()));
			ui.align(window.getView());

			inGamePosition = window.getView().getCenter();
			inGamePosition.x -= window.getView().getSize().x / 2;
			inGamePosition.x += Mouse::getPosition(window).x;
			inGamePosition.y -= window.getView().getSize().y / 2;
			inGamePosition.y += Mouse::getPosition(window).y;

			if (ui.update(info) == PlayerCustomizationUI::Changed)
			{
				player->setConfiguration(ui.getConfiguration());
			}

			window.clear();
			window.draw(cluster);
			world.draw(&window);
			window.draw(ui);
			window.display();			

			/*fps++;

			c.restart();*/
		/*}

		if (elapsedTime.getElapsedTime().asMilliseconds() >= 1000)
		{
			std::cout << "FPS: " << fps << "\n";

			fps = 0;

			elapsedTime.restart();
		}*/
		sleep(sleepDuration);
    }

    return 0;
}