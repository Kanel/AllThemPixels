#pragma once

#include "Shapes.h"
#include "Destructible.h"
#include <SFML\Graphics\Vertex.hpp>

using std::vector;
using namespace sf;

class Enemy : public Destructible
{
protected:
	int cooldown;
	int lastShootFired;
	Vertex shape[4];

public:
	Enemy(unsigned int hp, Vector2f position);

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