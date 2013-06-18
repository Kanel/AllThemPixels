#pragma once

#include "Entity.h"



class Destructible : public Entity
{
protected:
	unsigned int hp;

public:


	int getHP();
	void setHP(int hp);
	bool isAlive();
};