#include "Entity.h"

Entity::Entity(Vector2f position)
{
	this->position = position;
	this->type = UnknownEntity;
}

Vector2f Entity::getPosition()
{
	return position;
}

Vector2f Entity::getAim()
{
	return aim;
}

void Entity::setAim(Vector2f aim)
{
	this->aim = aim;
}

EntityTypes Entity::getType()
{
	return type;
}

void Entity::setPosition(Vector2f position)
{
	Vector2f diffrence = position - this->position;

	translate(diffrence);
}

void Entity::applyTransform(Transform &transform)
{
	position = transform.transformPoint(position);
}

void Entity::translate(Vector2f offset)
{
	Transform transform;

	transform.translate(offset);

	applyTransform(transform);
}

void Entity::rotate(float angle, Vector2f center)
{
	Transform transform;

	transform.rotate(angle, center);

	applyTransform(transform);
}

void Entity::scale(Vector2f factors, Vector2f center)
{
	Transform transform;

	transform.scale(factors, center);

	applyTransform(transform);
}