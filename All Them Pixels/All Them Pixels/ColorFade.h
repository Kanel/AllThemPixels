#pragma once

#include "UpdateInfo.h"
#include <SFML\Graphics.hpp>
#include <list>

using std::list;
using namespace sf;

struct Fade
{
	float r;
	float g;
	float b;
	float a;
	float delay;
	float age;
	float lifetime;
};

class ColorFade
{
private:
	Color color;
	list<Fade> fades;
	float rest[4];

public:
	ColorFade(Color color);

	void clear(Color color);
	void add(Fade fade);
	Color getColor();
	

	void update(UpdateInfo info);
};