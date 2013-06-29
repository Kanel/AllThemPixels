#include "Player.h"
#include "Territory.h"
#include "UpdateInfo.h"
#include "World.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

int main(int argc, char ** argv)
{
	char * windowtitle = "All them Pixels!";
	bool fullscreen = true;
	Uint32 windowStyle = Style::Resize;	
	ContextSettings settings(0, 0, 8, 2, 0);
	VideoMode videoMode(1024, 1024);
    RenderWindow window(videoMode, windowtitle, 7, settings);
	World world(Vector2f(), 512, 0, 3);
	sf::Clock c;
	sf::Clock elapsedTime;
	int fps = 0;
	Territory * t = world.getTerritory(AxialCoordinates(0, 0));
	Player * player = new Player(t, 100000000, Vector2f(0, 0));

	t->active = true;
	t->player = player;
	t->addEntity(player);

    while (window.isOpen())
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

		if (c.getElapsedTime().asMilliseconds() >= 10)
		{
			UpdateInfo info;
			View view = window.getView();

			info.elapsedSeconds = 10;
			info.ticks = 0;

			world.update(info);

			view.setCenter(player->getPosition());

			window.setView(view);
			window.clear();
			world.draw(&window);
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