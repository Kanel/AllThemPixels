#include "Enemy.h"

Enemy::Enemy(unsigned int hp, Vector2f position) : Destructible(hp, position)
{
	Shapes::rectangle(shape, 0, position, 20, 20, Color(0, 0, 0, 255));

	type = EntityTypes::EnemyEntity;
}

void Enemy::applyTransform(Transform transform)
{
	
}

Rect<float> Enemy::getBoundingBox()
{
	return Rect<float>(position.x - 10, position.y - 10, 20, 20);
}

ConvexHull Enemy::getConvexHull()
{
	vector<Vector2f> vertecies;

	for (int i = 0; i < 4; i++)
	{
		vertecies.push_back(shape[i].position);
	}

	return MonotoneChain::getConvexHull(vertecies);
}

void Enemy::draw(RenderWindow * window)
{
	window->draw(shape, 4, PrimitiveType::Quads);
}

void Enemy::update(UpdateInfo info)
{

}