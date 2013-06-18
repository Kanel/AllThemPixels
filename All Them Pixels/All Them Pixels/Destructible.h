#pragma once

#include "Entity.h"



class Destructible : public Entity
{
protected:
	unsigned int hp;

public:
	Destructible(usngined int);


	int getHP();
	void setHP(int hp);
	bool isAlive();
};