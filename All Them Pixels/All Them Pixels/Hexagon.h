#pragma once

#include <SFML\Graphics.hpp>
#include <math.h>

class Hexagon
{
protected:
	sf::VertexArray shape;

public:
	Hexagon();
	Hexagon(sf::Vector2f position, int radius);

	sf::VertexArray getShape();
};