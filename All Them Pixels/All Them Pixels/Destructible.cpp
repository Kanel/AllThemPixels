#include "Destructible.h"

Destructible::Destructible(unsigned int hp, Vector2f position) : Entity(position)
{
	this->hp = hp;
}

int Destructible::getHP()
{
	return hp;
}

void Destructible::setHP(int hp)
{
	this->hp = hp;
}

void Destructible::modHP(int hp)
{
	this->hp += hp;
}

bool Destructible::isExpended()
{
	return !(hp > 0);
}

bool Destructible::isEtheral()
{
	return false;
}