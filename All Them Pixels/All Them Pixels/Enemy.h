#pragma once

#include "Shapes.h"
#include "Destructible.h"
#include "Weapon.h"
#include "VertexCollection.h"
#include "SkillPoints.h"
#include "HexagonHull.h"
#include "Movement.h"
#include <SFML/Graphics/Vertex.hpp>

using std::vector;
using namespace sf;

struct AIProperties
{
	float aimAvoidanceDistance;
	float aimAvoidanceSpeed;
	float movementSpeed;
	float orbitalSpeed;
	float orbitalThreshold;
	float orbitalDistance;
	float stalkingDistance;
	float sinusRotationSpeed;
	float sinusRotationRadius;
};

class Enemy : public Destructible
{
protected:
	HexagonHull * hull;
	list<Movement *> movements;

public:
	AIProperties aiProperties;
	Weapon weapon;
	
public:
	Enemy(unsigned int hp, Vector2f position, HexagonHull * hull);
	~Enemy();

	void educate(AIProperties aiProperties);
	void arm(Weapon weapon);
	void equip(Movement * movement);

	SkillPoints getSkillPoints();

	void applyTransform(Transform &transform) override;

	Rect<float> getBoundingBox() override;
	ConvexHull getConvexHull() override;

	void update(UpdateInfo info, Controls * controls) override;
};