#pragma once

#include "Entity.h"

class Destructible : public Entity
{
protected:
	unsigned int hp;

public:
	Destructible(unsigned int hp, Vector2f position);

	int getHP();
	void setHP(int hp);

	bool isExpended() override;
	bool isEtheral() override;
};