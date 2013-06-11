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
				int h = event.size.height;
				int w = event.size.width;
				if (w > h)
				{
					shape.setRadius(h / 2);
					shape.setPosition(w / 2 - h / 2, 0);
				}
				else
				{
					shape.setRadius(w / 2);
					shape.setPosition(0, h / 2 - w / 2);
				}
				videoMode.height = h;
				videoMode.width = w;
				
				window.create(videoMode, windowtitle, windowStyle);
			}
        }
		
        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}