#pragma once

#define _USE_MATH_DEFINES

#include "Destructible.h"
#include "Projectile.h"
#include "UserInput.h"
#include "Weapon.h"
#include "Collision.h"
#include <queue>

using std::queue;

struct PlayerConfiguration
{
	WeaponConfiguration weaponConfig;
	float speed;
	int hp;
};

class Player : public Destructible
{
public:
	queue<Entity *> *spawnQueue;
	Vector2f aimVector;

protected:
	int shapeCount;
	int aimboxShapeCount;
	float aimDirection;
	Vector2f aimBoxPosition;
	Vertex shape[4];
	Vertex aimBoxShape[4];
	Weapon weapon;
	PlayerConfiguration config;

protected:
	void applyTransform(Transform transform, Vertex vertices[], int count);
	float getDirection(Joystick::Axis axisX, Joystick::Axis axisY);
	Vector2f getJoystickVector(Joystick::Axis x, Joystick::Axis y);
	void updateAim();

public:
	Player(queue<Entity *> *spawnQueue, PlayerConfiguration config, Vector2f position);

	void applyTransform(Transform transform) override;

	Rect<float> getBoundingBox() override;
	ConvexHull getConvexHull() override;
	void setConfiguration(PlayerConfiguration config);



	void fade();

	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};