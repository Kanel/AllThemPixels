#include <SFML/Graphics.hpp>
#include "HexagonGrid.h"
#include "GlareEffect.h"

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
	sf::Clock c;
	GlareEffect glare(Vector2f(0, 0));

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
			glare.rotate(0.5, glare.getPosition());
			glare.translate(Vector2f(0.5, 0.5));

			c.restart();
		}

        window.clear();
        window.draw(shape);
		glare.draw(&window);
        window.display();
    }

    return 0;
}