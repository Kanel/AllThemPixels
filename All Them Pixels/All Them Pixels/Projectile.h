#pragma once

#include "Entity.h"
#include "Shapes.h"

class Projectile : public Entity
{
protected:
	bool expended;
	Vertex shape[6];
	Vector2f speed;
	int ttl;

public:
	Projectile(Vector2f position, Vector2f speed, int ttl);

	bool isExpended() override;

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};