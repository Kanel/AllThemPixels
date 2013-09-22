#include "LinearMovement.h"

LinearMovement::LinearMovement(Vector2f speed)
{
	this->speed = speed;
}

void LinearMovement::update(UpdateInfo info)
{
	*position += speed * (info.updateInterval / 1000.0f);
}
