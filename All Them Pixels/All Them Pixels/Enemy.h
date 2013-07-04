#pragma once

#include "Shapes.h"
#include "Destructible.h"
#include "Weapon.h"
#include <SFML\Graphics\Vertex.hpp>

using std::vector;
using namespace sf;

struct AIProperties
{
	float speed;
	int aimavoidance;
	float playerspace;
	bool righthanded;
};

class Enemy : public Destructible
{
protected:
	int cooldown;
	int lastShootFired;
	Vertex shape[4];

public:
	AIProperties aiProperties;
	Weapon weapon;

public:
	Enemy(unsigned int hp, Vector2f position);
	void educate(AIProperties aiProperties);
	void arm(Weapon weapon);

	int getCooldown();
	void setCooldown(int cooldown);

	int getLastShootFired();
	void setLastShootFired(int lastShootFired);

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;
	ConvexHull getConvexHull() override;

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};