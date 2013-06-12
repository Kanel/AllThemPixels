#include <SFML/Graphics.hpp>
#include "HexagonGrid.h"

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
	HexagonGrid grid(Vector2f(250, 250), 5, 50, -6);
	
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
		
        window.clear();		
        window.draw(shape);
		grid.draw(&window);
        window.display();
    }

    return 0;
}