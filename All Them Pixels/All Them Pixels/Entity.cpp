#include "Entity.h"

Entity::Entity(Vector2f position)
{
	this->position = position;

	type = EntityTypes::UnknownEntity;
}

Vector2f Entity::getPosition()
{
	return position;
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

void Entity::applyTransform(Transform transform)
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

bool Entity::collidesWith(Entity * other)
{
	bool collision = Shapes::contains(getBoundingBox(), other->getBoundingBox());
	
	return collision && collidesWith(other->getConvexHull());
}

bool Entity::collidesWith(ConvexHull convexHull)
{
	list<Vector2f> penetration;

	return SAT::collides(getConvexHull(), convexHull, Vector2f(), penetration);
}