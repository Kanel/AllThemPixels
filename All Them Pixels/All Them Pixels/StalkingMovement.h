#pragma once

#include "TrackingMovement.h"
#include "Entity.h"

using namespace sf;

/**
 * Movement that stalks a target while trying to maintain a preferred distance to it. 
 */
class StalkingMovement : public Movement
{
private:
	Entity * target; /**< The entity to stalk. */
	float speed; /**< The movement speed in pixels per second. */
	float distance; /**< The preferred distance to the target in pixels. */

public:
	/**
	 * @param target The entity to stalk.
	 * @param speed The movement speed in pixels per second.
	 * @param distance The preferred distance to keep from the target in pixels.
	 */
	StalkingMovement(Entity * target, float speed, float distance);

	/**
	 * Causes movement towards the preferred distance of the target entity.
	 * @param info Information about the update.
	 */
	virtual void update(UpdateInfo info);
};