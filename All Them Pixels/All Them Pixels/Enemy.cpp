#include "Enemy.h"

Enemy::Enemy(unsigned int hp, Vector2f position, VertexCollection * vertexSource) : Destructible(hp, position)
{
	Vertex vertices[6];

	this->vertexSource = vertexSource;

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

void Enemy::update(UpdateInfo info)
{
	
}