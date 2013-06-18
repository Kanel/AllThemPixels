#pragma once

#define _USE_MATH_DEFINES

#include "Destructible.h"
#include <math.h>

class Player : public Destructible
{
protected:
	float aimDirection;
	Vector2f aimBoxPosition;
	VertexArray shape;
	VertexArray aimBox;

protected:
	void applyTransform(Transform transform, VertexArray &vertecies);
	float getDirection(Joystick::Axis axisX, Joystick::Axis axisY);
	Vector2f getJoystickVector(Joystick::Axis x, Joystick::Axis y);
	void updateAim();

public:
	Player(unsigned int hp, Vector2f position);

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};