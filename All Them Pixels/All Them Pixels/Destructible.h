#pragma once

#include "Entity.h"

class Destructible : public Entity
{
protected:
	int hp;

public:
	Destructible(int hp, Vector2f position);

	int getHP();
	void setHP(int hp);
	void modHP(int hp);

	void expend() override;
	bool isExpended() override;
	bool isEtheral() override;
};