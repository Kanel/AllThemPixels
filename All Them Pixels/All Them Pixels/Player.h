#pragma once

#define _USE_MATH_DEFINES

#include "Territory.h"
#include "Destructible.h"
#include "Projectile.h"
#include "UserInput.h"
#include "Weapon.h"
#include <math.h>

using std::vector;

class Territory;

class Player : public Destructible
{
public:
	Territory * removePlease;

protected:
	int shapeCount;
	int aimboxShapeCount;
	float aimDirection;
	Vector2f aimBoxPosition;
	Vertex shape[4];
	Vertex aimBoxShape[4];
	Weapon weapon;

protected:
	void applyTransform(Transform transform, Vertex vertices[], int count);
	float getDirection(Joystick::Axis axisX, Joystick::Axis axisY);
	Vector2f getJoystickVector(Joystick::Axis x, Joystick::Axis y);
	void updateAim();

public:
	Player(Territory * removePlease, unsigned int hp, Vector2f position);

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;
	ConvexHull getConvexHull() override;

	void fade();

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};