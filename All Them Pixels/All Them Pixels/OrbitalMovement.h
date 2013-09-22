#pragma once

#include "Entity.h"
#include "Movement.h"

/**
 * Movement that stalks a target while trying to maintain a preferred distance to it. 
 */
class OrbitalMovement : public Movement
{
private:
	Entity * target; /**< The entity to stalk. */
	float speed; /**< The movement speed in pixels per second. */
	float distance; /**< The preferred orbital distance to the target in pixels. */
	float threshold; /**< The allowed deviation from the preferred distance in pixels. */

public:
	/**
	 * @param target The entity to stalk.
	 * @param speed The movement speed in pixels per second.
	 * @param distance The preferred orbital distance to keep from the target in pixels.
	 * @param threshold The allowed deviation from the preferred distance in pixels.
	 */
	OrbitalMovement(Entity * target, float speed, float distance, float threshold);

	/**
	 * Causes movement towards the preferred distance of the target entity.
	 * @param info Information about the update.
	 */
	virtual void update(UpdateInfo info);
};