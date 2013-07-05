#include "Enemy.h"

Enemy::Enemy(unsigned int hp, Vector2f position, VertexCollection * vertexSource) : Destructible(hp, position)
{
	Vertex vertices[6];

	this->vertexSource = vertexSource;

	cooldown = 500 + rand() % 256;
	lastShootFired = 0;
	type = EntityTypes::EnemyEntity;
	vertexCount = 4;	

	Shapes::rectangle(vertices, 0, position, 20, 20, Color(0, 0, 160, 255));

	vertexOffset = vertexSource->add(vertices);
}

Enemy::~Enemy()
{
	vertexSource->remove(vertexOffset);
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

	for (int i = 0; i < vertexCount; i++)
	{
		(*vertexSource)[vertexOffset + i].position = transform.transformPoint((*vertexSource)[vertexOffset + i].position);
	}
}

Rect<float> Enemy::getBoundingBox()
{
	return Rect<float>(position.x - 10, position.y - 10, 20, 20);
}

ConvexHull Enemy::getConvexHull()
{
	vector<Vector2f> vertices;

	for (int i = 0; i < vertexCount; i++)
	{
		vertices.push_back((*vertexSource)[vertexOffset + i].position);
	}

	return MonotoneChain::getConvexHull(vertices);
}

void Enemy::draw(RenderWindow * window)
{
	//window->draw(shape, 4, PrimitiveType::Quads);
}

void Enemy::update(UpdateInfo info)
{
	
}