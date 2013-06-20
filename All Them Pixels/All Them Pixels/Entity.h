#pragma once

#include "EntityTypes.h"
#include "Shapes.h"
#include "UpdateInfo.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class Entity
{
protected:
	Vector2f position;
	EntityTypes type;

public:
	Entity(Vector2f position);

	virtual Vector2f getPosition();
	virtual void setPosition(Vector2f position);

	EntityTypes getType();
	virtual bool isExpended() = NULL;

	virtual void applyTransform(Transform transform);
	virtual void translate(Vector2f offset);
	virtual void rotate(float angle, Vector2f center);
	virtual void scale(Vector2f factors, Vector2f center);

	virtual Rect<float> getBoundingBox() = NULL;
	virtual bool collidesWith(Entity * other);

	virtual void draw(RenderWindow * window) = NULL;
	virtual void update(UpdateInfo info) = NULL;
};