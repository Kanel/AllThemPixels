#pragma once

#include "Destructible.h"

class Enemy : public Destructible
{
public:
	Enemy(unsigned int hp, Vector2f position);

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};