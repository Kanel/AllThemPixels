#include "AimAvoidanceMovement.h"

Vector2f AimAvoidanceMovement::aproximation(Vector2f vector)
{
	return Vector2f(floorf(vector.x * 10000) / 10000, floorf(vector.y * 10000) / 10000);
}

AimAvoidanceMovement::AimAvoidanceMovement(Entity * target, float speed, float distance)
{
	this->target = target;
	this->position = NULL;
	this->speed = speed;
	this->distance = distance;
}

void AimAvoidanceMovement::update(UpdateInfo info)
{
	Vector2f direction = Vector2fMath::unitVector(target->getPosition() - *position);
	float length = Vector2fMath::length(target->getPosition() - *position);
	float movement = Vector2fMath::length(direction * speed * (info.updateInterval / 1000.0f));

	Vector2f a = target->getPosition();
	Vector2f p = target->getPosition();
	Vector2f n = target->getAim();
	Vector2f v1 = Vector2fMath::dot((a-p), n)*n;
	Vector2f v2 = aproximation(Vector2fMath::invert(Vector2fMath::unitVector(v1)));
	Vector2f v3 = aproximation(n);
	float distance = Vector2fMath::length((a-p)-v1);

	if (distance < this->distance && v3 == v2)
	{
		direction = Vector2fMath::invert(Vector2fMath::unitVector((a-p)-v1));
	}
}
