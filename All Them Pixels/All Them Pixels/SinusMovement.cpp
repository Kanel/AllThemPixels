#include "SinusMovement.h"

SinusMovement::SinusMovement(float rotationSpeed, float rotationRadius)
{
	this->rotationSpeed = rotationSpeed;
	this->rotationRadius = rotationRadius;
	this->rotation = 0;
	this->position = NULL;
}

void SinusMovement::setPosition(Vector2f * position)
{
	this->position = position;
	this->previousPosition = *position;
}

void SinusMovement::update(UpdateInfo info)
{
	float timeFraction = info.updateInterval / 1000.0f;
	Vector2f leftNormal = Vector2fMath::leftNormal(Vector2fMath::unitVector(*position - previousPosition));
	Vector2f movement = Vector2f(cosf(rotation) * rotationRadius * timeFraction, sinf(rotation) * rotationRadius * timeFraction);

	previousPosition = *position;	
	rotation += rotationSpeed * timeFraction;
	//*position -= previousMovement;
	*position += movement;
	previousMovement = movement;
}
