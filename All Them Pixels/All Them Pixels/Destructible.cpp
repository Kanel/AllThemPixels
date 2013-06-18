#include "Destructible.h"

int Destructible::getHP()
{
	return hp;
}

void Destructible::setHP(int hp)
{
	this->hp = hp;
}

bool Destructible::isAlive()
{
	return hp > 0;
}