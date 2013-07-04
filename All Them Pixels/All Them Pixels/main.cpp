#include "Player.h"
#include "Territory.h"
#include "UpdateInfo.h"
#include "World.h"
#include "PlayerCustomizationUI.h"
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
	Clock c;
	Clock elapsedTime;
	int fps = 0;
	Territory * t = world.getTerritory(AxialCoordinates(0, 0));
	Player * player;
	PlayerConfiguration playerconfig;
	PlayerCustomizationUI ui(Vector2f(0, 0));

	playerconfig.hp = 10000;
	playerconfig.speed = 1.5;
	playerconfig.weaponConfig.cooldown = 100;
	playerconfig.weaponConfig.damage = 100;
	playerconfig.weaponConfig.piercing = 1;
	playerconfig.weaponConfig.speed = 10;
	playerconfig.weaponConfig.spread = 5;
	playerconfig.weaponConfig.ttl = 55;

	player = new Player(t, playerconfig, Vector2f(0, 0));

	t->active = true;
	t->player = player;
	t->addEntity(player);

	while (world.isActive())
    {
        Event event;

        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
			{
                window.close();
			}
			if (event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::F11))
			{
				windowStyle = (windowStyle == Style::Fullscreen ? Style::Resize | Style::Close : Style::Fullscreen);

				window.create(videoMode, windowtitle, windowStyle);
			}
			if (event.type == Event::Resized)
			{
				videoMode.height = event.size.height;
				videoMode.width = event.size.width;
				
				window.create(videoMode, windowtitle, windowStyle);
			}
        }

		if (c.getElapsedTime().asMilliseconds() >= updateInterval)
		{
			UpdateInfo info;
			Vector2f inGamePosition;

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

			if (ui.update(inGamePosition) == PlayerCustomizationUI::Changed)
			{
				player->setConfiguration(ui.getConfiguration());
			}

			window.clear();
			world.draw(&window);
			window.draw(ui);
			window.display();			

			fps++;

			c.restart();
		}

		if (elapsedTime.getElapsedTime().asMilliseconds() >= 1000)
		{
			std::cout << "FPS: " << fps << "\n";

			fps = 0;

			elapsedTime.restart();
		}
    }

    return 0;
}