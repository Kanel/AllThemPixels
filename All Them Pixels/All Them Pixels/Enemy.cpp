#include "Enemy.h"

Enemy::Enemy(unsigned int hp, Vector2f position) : Destructible(hp, position)
{
	Shapes::rectangle(shape, 0, position, 20, 20, Color(0, 0, 160, 255));

	cooldown = 500 + rand() % 256;
	lastShootFired = 0;
	type = EntityTypes::EnemyEntity;
}

void Enemy::educate(AIProperties aiProperties)
{
	this->aiProperties = aiProperties;
}

void Enemy::arm(Weapon weapon)
{
	this->weapon = weapon;
}

int Enemy::getCooldown()
{
	return cooldown;
}

void Enemy::setCooldown(int cooldown)
{
	this->cooldown = cooldown;
}

int Enemy::getLastShootFired()
{
	return lastShootFired;
}

void Enemy::setLastShootFired(int lastShootFired)
{
	this->lastShootFired = lastShootFired;
}

void Enemy::applyTransform(Transform transform)
{
	Destructible::applyTransform(transform);

	for (int i = 0; i < 4; i++)
	{
		shape[i].position = transform.transformPoint(shape[i].position);
	}
}

Rect<float> Enemy::getBoundingBox()
{
	return Rect<float>(position.x - 10, position.y - 10, 20, 20);
}

ConvexHull Enemy::getConvexHull()
{
	vector<Vector2f> vertices;

	for (int i = 0; i < 4; i++)
	{
		vertices.push_back(shape[i].position);
	}

	return MonotoneChain::getConvexHull(vertices);
}

void Enemy::draw(RenderWindow * window)
{
	window->draw(shape, 4, PrimitiveType::Quads);
}

void Enemy::update(UpdateInfo info)
{
	
}