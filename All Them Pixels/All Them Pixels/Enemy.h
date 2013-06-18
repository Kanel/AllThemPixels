#pragma once

#include "Entity.h"

class Enemy : Entity
{
public:
	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};