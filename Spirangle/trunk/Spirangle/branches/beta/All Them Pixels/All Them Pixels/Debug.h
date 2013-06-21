#pragma once

#include <SFML\Graphics\Text.hpp>

using namespace sf;

class Debug
{
protected:
	Vector2f position;
	Font font;

public:
	Debug();

	void print(String text);
};