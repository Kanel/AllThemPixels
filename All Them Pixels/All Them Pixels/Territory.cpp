#include "Territory.h"

Territory::Territory(Vector2f position, int radius)
{
	shape.top = position.y - radius;
	shape.left = position.x - radius;
	shape.width = radius * 2;
	shape.height = radius * 2;

	Shapes::hexagon(border,0,position,400,Color(250,250,250,100));
	//Shapes::rectangle(border, 0, position, radius * 2, radius * 2);

	border[6] = border[0];
	Color colors[4] = 
	{
		Color(250,0,0,30),
		Color(0,250,0,30),
		Color(0,0,250,30),
		Color(100,100,100,30)
	};
	floorTiles = FloorTiles(Vector2i(1024,1024), colors);
}

Territory::~Territory()
{
	integrateSpawnQueue();

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end();)
	{
		Entity * entity = *it;

		it = entities.erase(it);

		delete entity;
	}
}

void Territory::addEntity(Entity * entity)
{
	spawnQueue.push(entity);
}

void Territory::removeEntity(Entity * entity)
{
	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end();)
	{
		if ((*it) == entity)
		{
			Entity * entity = *it;

			it = entities.erase(it);

			delete entity;
		}
		else
		{
			it++;
		}
	}
}

void Territory::integrateSpawnQueue()
{
	while(spawnQueue.size() > 0)
	{
		entities.push_back(spawnQueue.front());
		spawnQueue.pop();
	}
}

void Territory::cleanup()
{
	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end();)
	{
		if (!(*it)->isAlive())
		{
			Entity * entity = *it;

			it = entities.erase(it);

			delete entity;
		}
		else
		{
			it++;
		}
	}
}

void Territory::draw(RenderWindow * window)
{
	int radius = 900;
	float width = sqrt(3 * (radius * radius) / 4);
	Vertex background[6] = 
	{
		Vertex(Vector2f(512, 512 - radius), Color::Blue),
		Vertex(Vector2f(512 + width, 512 - (radius/2)), Color::Green),
		Vertex(Vector2f(512 + width, 512 + (radius/2)), Color::Magenta),
		Vertex(Vector2f(512, 512 + radius), Color::Red),
		Vertex(Vector2f(512 - width, 512 + (radius/2)), Color::Yellow),
		Vertex(Vector2f(512 - width, 512 - (radius/2)), Color::Black)
	};
	window->draw(background, 6, PrimitiveType::TrianglesFan);
	window->draw(border, 7, PrimitiveType::LinesStrip);
	floorTiles.draw(window);

	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		(*it)->draw(window);
	}
}

void Territory::update(UpdateInfo info)
{
	Vector2f vector(Joystick::getAxisPosition(0, Joystick::Axis::X), 
					Joystick::getAxisPosition(0, Joystick::Axis::Y));

	vector.x = (powf(vector.x, 2) > 400) ? -vector.x / 15 : 0;
	vector.y = (powf(vector.y, 2) > 400) ? -vector.y / 15 : 0;

	floorTiles.move(vector);
	for (std::list<Entity *>::iterator it = entities.begin(); it != entities.end(); it++)
	{
		(*it)->update(info);
	}
}