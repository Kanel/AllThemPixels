//#include "HexagonGrid.h"
//#include "GlareEffect.h"
#include "Player.h"
#include "Territory.h"
#include "UpdateInfo.h"
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
	Territory core(Vector2f(0, 0), 512);
	sf::Clock c;
	sf::Clock elapsedTime;
	int fps = 0;
	Player * player = new Player(&core, 100, Vector2f(0, 0));

	core.player = player;
	core.addEntity(player);

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

			core.integrateSpawnQueue();
			core.update(info);
			core.cleanup();

			view.setCenter(player->getPosition());

			window.setView(view);
			window.clear();
			core.draw(&window);
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