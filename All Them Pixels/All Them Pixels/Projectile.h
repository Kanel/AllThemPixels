#pragma once

#include "Destructible.h"
#include "Shapes.h"

class Projectile : public Entity
{
protected:
	Vertex shape[4];
	Vector2f speed;

public:
	Projectile(Vector2f position, Vector2f speed);

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};