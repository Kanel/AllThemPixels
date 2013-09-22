#include "OrbitalMovement.h"

OrbitalMovement::OrbitalMovement(Entity * target, float speed, float distance, float threshold)
{
	this->target = target;
	this->position = NULL;
	this->speed = speed;
	this->distance = distance;
	this->threshold = threshold;
}

void OrbitalMovement::update(UpdateInfo info)
{
	float length = Vector2fMath::length(target->getPosition() - *position);	
	
	if (distance - threshold <= length && length <= distance + threshold)
	{
		Vector2f direction = Vector2fMath::unitVector(target->getPosition() - *position);
		Vector2f movement = Vector2fMath::rightNormal(direction) * (speed * (info.updateInterval / 1000.0f));

		*position -= direction + movement;
	}
}
