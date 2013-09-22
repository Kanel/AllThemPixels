#pragma once

#include "Movement.h"
#include "Entity.h"
#include "Vector2fMath.h"

using namespace sf;

/**
 * Movement that follows a entity.
 */
class TrackingMovement : public Movement
{
private:
	Entity * target; /**< The entity to track. */
	float speed; /**< The movement speed in pixels per second. */

public:
	/**
	 * @param target The entity to track.
	 * @param speed The movement speed in pixels per second.
	 */
	TrackingMovement(Entity * target, float speed);

	/**
	 * Causes movement towards the target entity.
	 * @param info Information about the update.
	 */
	virtual void update(UpdateInfo info);
};