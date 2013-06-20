#include "Enemy.h"

Enemy::Enemy(unsigned int hp, Vector2f position) : Destructible(hp, position)
{
	Shapes::rectangle(shape, 0, position, 20, 20);

	type = EntityTypes::EnemyEntity;
}

void Enemy::applyTransform(Transform transform)
{
	
}

Rect<float> Enemy::getBoundingBox()
{
	return Rect<float>(position.x - 10, position.y - 10, 20, 20);
}

void Enemy::draw(RenderWindow * window)
{
	window->draw(shape, 4, PrimitiveType::Quads);
}

void Enemy::update(UpdateInfo info)
{

}