#pragma once

#include "Destructible.h"

class Player : public Destructible
{
public:
	Player(unsigned int hp, Vector2f position);

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};