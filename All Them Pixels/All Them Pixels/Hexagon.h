#pragma once

#include <SFML\Graphics.hpp>
#include <math.h>

class Hexagon
{
protected:
	sf::VertexArray shape;

public:
	Hexagon(sf::Vector2f position);

	sf::VertexArray getShape();
};