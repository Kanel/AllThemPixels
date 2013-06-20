#pragma once

#include "Shapes.h"
#include "Destructible.h"
#include <SFML\Graphics\Vertex.hpp>

using namespace sf;

class Enemy : public Destructible
{
protected:
	Vertex shape[4];

public:
	Enemy(unsigned int hp, Vector2f position);

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};