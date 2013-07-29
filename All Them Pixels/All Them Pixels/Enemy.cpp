#include "Enemy.h"

Enemy::Enemy(unsigned int hp, Vector2f position, HexagonHull * hull) : Destructible(hp, position)
{
	type = EnemyEntity;
	this->hull = hull;
}

Enemy::~Enemy()
{
	delete hull;
}

void Enemy::educate(AIProperties aiProperties)
{
	this->aiProperties = aiProperties;
}

void Enemy::arm(Weapon weapon)
{
	this->weapon = weapon;
}

SkillPoints Enemy::getSkillPoints()
{
	SkillPoints points;

	points.amount = 10;
	points.type = SkillPointType::Common;

	return points;
}

void Enemy::applyTransform(Transform transform)
{
	Destructible::applyTransform(transform);

	hull->applyTransform(transform);
}

Rect<float> Enemy::getBoundingBox()
{
	return Rect<float>(position.x - 10, position.y - 10, 20, 20);
}

ConvexHull Enemy::getConvexHull()
{
	vector<Vector2f> vertices;

	/*for (int i = 0; i < vertexCount; i++)
	{
		vertices.push_back((*vertexSource)[vertexOffset + i].position);
	}*/
	assert(false && "Cannot be used, fix it!");

	return MonotoneChain::getConvexHull(vertices);
}

void Enemy::update(UpdateInfo info)
{
	
}