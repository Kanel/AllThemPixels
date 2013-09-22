#include "Enemy.h"

Enemy::Enemy(unsigned int hp, Vector2f position, HexagonHull * hull) : Destructible(hp, position)
{
	type = EnemyEntity;
	this->hull = hull;
}

Enemy::~Enemy()
{
	delete hull;

	for (auto movement : movements)
	{
		delete movement;
	}
}

void Enemy::educate(AIProperties aiProperties)
{
	this->aiProperties = aiProperties;
}

void Enemy::arm(Weapon weapon)
{
	this->weapon = weapon;
}

void Enemy::equip(Movement * movement)
{
	movements.push_back(movement);
	movement->setPosition(&position);
}

SkillPoints Enemy::getSkillPoints()
{
	SkillPoints points;

	points.amount = 10;
	points.type = SkillPointType::Common;

	return points;
}

void Enemy::applyTransform(Transform &transform)
{
	Destructible::applyTransform(transform);

	hull->applyTransform(transform);
}

Rect<float> Enemy::getBoundingBox()
{
	return hull->getBoundingBox();
}

ConvexHull Enemy::getConvexHull()
{
	return hull->getConvexHull();
}

void Enemy::update(UpdateInfo info, Controls * controls)
{
	for (auto movement : movements)
	{
		movement->update(info);
	}
	hull->setPosition(position);
}