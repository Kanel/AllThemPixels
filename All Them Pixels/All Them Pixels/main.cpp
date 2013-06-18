#include "HexagonGrid.h"
#include "GlareEffect.h"
#include "Player.h"
#include "Territory.h"
#include "UpdateInfo.h"
#include <SFML/Graphics.hpp>

using namespace sf;

int main(int argc, char ** argv)
{
	char * windowtitle = "All them Pixels!";
	bool fullscreen = true;
	Uint32 windowStyle = Style::Resize;	
	ContextSettings settings(0, 0, 8, 2, 0);
	VideoMode videoMode(500, 500);
    RenderWindow window(videoMode, windowtitle, 7, settings);
    CircleShape shape(100.f);
	Territory core(Vector2f(250, 250), 125);
	sf::Clock c;

	core.addEntity(new Player(100, Vector2f(250, 250)));
	core.addEntity(new GlareEffect(Vector2f(400, 100)));
    shape.setFillColor(Color::Green);

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
				int height = event.size.height;
				int width = event.size.width;

				if (width > height)
				{
					shape.setRadius(height / 2);
					shape.setPosition(width / 2 - height / 2, 0);
				}
				else
				{
					shape.setRadius(width / 2);
					shape.setPosition(0, height / 2 - width / 2);
				}
				shape.setPointCount(3.14 * 2 * shape.getRadius() / 20);

				videoMode.height = height;
				videoMode.width = width;
				
				window.create(videoMode, windowtitle, windowStyle);
			}
        }

		if (c.getElapsedTime().asMilliseconds() >= 20)
		{
			UpdateInfo info;

			info.elapsedSeconds = 20;

			core.integrateSpawnQueue();
			core.update(info);

			window.clear();
			window.draw(shape);
			core.draw(&window);
			window.display();

			c.restart();
		}
    }

    return 0;
}