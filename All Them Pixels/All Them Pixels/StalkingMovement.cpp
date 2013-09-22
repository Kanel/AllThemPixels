#include "StalkingMovement.h"

StalkingMovement::StalkingMovement(Entity * target, float speed, float distance)
{
	this->target = target;
	this->position = NULL;
	this->speed = speed;
	this->distance = distance;
}

void StalkingMovement::update(UpdateInfo info)
{
	Vector2f direction = Vector2fMath::unitVector(target->getPosition() - *position);
	float length = Vector2fMath::length(target->getPosition() - *position);
	float movement = Vector2fMath::length(direction * speed * (info.updateInterval / 1000.0f));

	if (length - movement <= distance)
	{
		// Correct by moving towards the preferred radius.
		float correction = distance - length;

		if (correction >= movement)
		{
			*position -= direction * movement;
		}
		else
		{
			*position -= direction * correction;
		}
	}
	else
	{
		*position += direction * speed * (info.updateInterval / 1000.0f);
	}
}
