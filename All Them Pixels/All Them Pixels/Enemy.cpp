#include "Enemy.h"

Enemy::Enemy(unsigned int hp, Vector2f position) : Destructible(hp, position)
{

}

void Enemy::applyTransform(Transform transform)
{
	
}

Rect<float> Enemy::getBoundingBox()
{
	return Rect<float>();
}

void Enemy::draw(RenderWindow * window)
{

}

void Enemy::update(UpdateInfo info)
{

}