#include <SFML/Graphics.hpp>

using namespace sf;

int main(int argc, char ** argv)
{
	char * windowtitle = "All them Pixels!";
	bool fullscreen = true;
	Uint32 windowStyle = Style::Resize;
	VideoMode videoMode(200, 200);
    RenderWindow window(videoMode, windowtitle);
    CircleShape shape(100.f);
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
				shape.setPointCount(3.14 * 2 * shape.getRadius() * 100000);

				videoMode.height = height;
				videoMode.width = width;
				
				window.create(videoMode, windowtitle, windowStyle);
			}
        }
		
        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}