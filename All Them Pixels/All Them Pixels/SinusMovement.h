#pragma once

#include "LinearMovement.h"
#include "Vector2fMath.h"
#include <math.h>

/** 
 * Movement in a sinus wave like manner i.e continuously moving left and right in an interval.
 */
class SinusMovement : public Movement
{
private:
	float rotation; /**< The current rotation in radians. */
	float rotationSpeed; /**< The angular speed in radians per second. */
	float rotationRadius; /**< The radius of the left and right movement in pixels. */
	Vector2f previousMovement; /**< The movement that was caused during the preious update. */
	Vector2f previousPosition;

public:
	/** 
	 * @param rotationSpeed The angular speed in radians per second.
	 * @param rotationRadius The radius of the left and right movement in pixels.
	 */
	SinusMovement(float rotationSpeed, float rotationRadius);
	
	/**
     * Sets the position which should be manipulated.
	 * @param position The position to inflict movement onto.
	 */
	virtual void setPosition(Vector2f * position);

	/** 
	 * Causes sinus wave like movement.
	 * @param info Information about the update.
	 */
	virtual void update(UpdateInfo info);
};