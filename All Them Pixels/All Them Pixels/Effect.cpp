#include "Effect.h"

Effect::Effect(int start, int duration)
{
	this->start = start;
	this->duration = duration;
}

Effect::~Effect() { }

Vector2f Effect::getPosition()
{
	return position;
}

void Effect::setPosition(Vector2f position)
{
	this->position = position;
}

bool Effect::isExpended()
{
	return expended;
}