#include "TrackingMovement.h"

TrackingMovement::TrackingMovement(Entity * target, float speed)
{
	this->target = target;
	this->speed = speed;
	this->position = NULL;
}

void TrackingMovement::update(UpdateInfo info)
{
	Vector2f direction = Vector2fMath::unitVector(target->getPosition() - *position);

	*position += direction * speed * (info.updateInterval / 1000.0f);
}
