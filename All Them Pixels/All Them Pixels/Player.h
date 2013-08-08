#pragma once

#define _USE_MATH_DEFINES

#include "Destructible.h"
#include "Projectile.h"
#include "UserInput.h"
#include "Weapon.h"
#include "Collision.h"
#include "SkillPoints.h"
#include "Sounds.h"
#include "CircleBar.h"
#include <queue>

using std::queue;

struct PlayerConfiguration
{
	WeaponConfiguration weaponConfig;
	int speed;
	int hp;
};

struct PlayerSkillPoints
{
	int common;
	int commonUsed;
};

class Player : public Destructible, public Drawable
{
public:
	queue<Entity *> *spawnQueue;
	Vector2f aimVector;

protected:
	int shapeCount;
	int aimboxShapeCount;
	float aimDirection;
	int safeZoneIndex;
	bool isInSafeZone;
	Vector2f aimBoxPosition;
	Weapon weapon;
	PlayerConfiguration config;
	Vector2f speed;
	PlayerSkillPoints playerSkillPoints;
	CircleBar healthBar;
	VertexCollection projectileVertexSource;
	int leftIndex;
	int rightIndex;
	int bottomIndex;
	int coreIndex;
	int playerVertexCount;
	int leftVertexCount;
	int rightVertexCount;
	int bottomVertexCount;
	int coreVertexCount;
	Vertex playerShape[57];
	Vertex * playerShapeCore;
	Vertex * playerShapeBottom;
	Vertex * playerShapeLeft;
	Vertex * playerShapeRight;

protected:
	void applyTransform(Transform transform, Vertex vertices[], int count);
	void applyColor(Color color, Vertex vertices[], int count);
	float getDirection(Joystick::Axis axisX, Joystick::Axis axisY);
	float getStrength(Joystick::Axis x, Joystick::Axis y);
	void updateRotation();

public:
	Player(queue<Entity *> *spawnQueue, PlayerConfiguration config, Vector2f position);

	bool getIsInSafeZone();
	void setIsInSafeZone(bool isInSafeZone);

	int getSafeZoneIndex();
	void setSafeZoneIndex(int safeZoneIndex);

	Vector2f getSpeed();

	void applyTransform(Transform transform) override;

	void setHP(int hp) override;

	Rect<float> getBoundingBox() override;
	ConvexHull getConvexHull() override;
	void setConfiguration(PlayerConfiguration config);

	void addSkillPoints(SkillPoints skillPoints);
	PlayerSkillPoints * getPlayerSkillPoints();

	void fade();

	virtual void draw(RenderTarget& target, RenderStates states) const;
	void update(UpdateInfo info) override;
};