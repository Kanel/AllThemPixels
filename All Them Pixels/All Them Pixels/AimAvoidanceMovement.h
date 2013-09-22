#pragma once

#include "Movement.h"
#include "Entity.h"
#include "Vector2fMath.h"

using namespace sf;

/**
 * Movement that tries to evade line of fire.
 */
class AimAvoidanceMovement : public Movement
{
private:
	Entity * target; /**< The entity whose line of fire should be evaded. */
	float speed; /**< The movement speed in pixels per second. */
	float distance; /**< The preferred distance to keep from the line of fire in pixels. */

private:
	Vector2f aproximation(Vector2f vector);

public:
	/**
	 * @param target The entity whose line of fire should be evaded.
	 * @param speed The movement speed in pixels per second.
	 * @distance distance The preferred distance to keep from the line of fire in pixels.
	 */
	AimAvoidanceMovement(Entity * target, float speed, float distance);

	/**
	 * Causes movement away fromthe line of fire of the target.
	 * @param info Information about the update.
	 */
	virtual void update(UpdateInfo info);
};