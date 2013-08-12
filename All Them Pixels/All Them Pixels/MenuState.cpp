#include "MenuState.h"

MenuState::MenuState()
{
	paused = false;
}

MenuState::~MenuState()
{

}

void MenuState::pause()
{
	paused = true;
}

void MenuState::resume()
{
	paused = false;
}

bool MenuState::blocking()
{
	return true;
}

void MenuState::handleEvents(GameEngine * engine, vector<Event> events)
{
	for (int i = 0; i < events.size(); i++)
	{
		if (events[i].type == Event::KeyPressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				expend();
			}
		}
	}
}

void MenuState::update(GameEngine * engine, UpdateInfo info)
{

}

void MenuState::draw(RenderTarget& target, RenderStates states) const
{
	Color color(0, 0, 0, 120);
	Vertex vertices[4];
	Vector2f center = target.getView().getCenter();
	Vector2f size = target.getView().getSize();

	vertices[0] = Vertex(Vector2f(center.x - size.x / 2, center.y + size.y / 2), color);
	vertices[1] = Vertex(Vector2f(center.x + size.x / 2, center.y + size.y / 2), color);
	vertices[2] = Vertex(Vector2f(center.x + size.x / 2, center.y - size.y / 2), color);
	vertices[3] = Vertex(Vector2f(center.x - size.x / 2, center.y - size.y / 2), color);

	target.draw(vertices, 4, PrimitiveType::Quads);
}