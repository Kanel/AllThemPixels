#pragma once

#include "UpdateInfo.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class Effect
{
protected:
	int start;
	int duration;
	int age;
	bool expended;
	Vector2f position;

public:
	Effect(int start, int duration);
	virtual ~Effect();

	virtual Vector2f getPosition();
	virtual void setPosition(Vector2f position);

	virtual bool isExpended();
	virtual void update(UpdateInfo info) = 0;
};