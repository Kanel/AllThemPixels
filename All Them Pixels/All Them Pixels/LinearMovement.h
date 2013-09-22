#pragma once

#include "Movement.h"

using namespace sf;

/**
 * Movement in a straight line.
 */
class LinearMovement : public Movement
{
private:
	Vector2f speed; /**< The movement speed in pixels per second. */

public:
	/**
	 * @param speed The movement speed in pixels per second.
	 */
	LinearMovement(Vector2f speed);

	/** Causes movement in a straight line. 
	 * @param info Information about the update.
	 */
	virtual void update(UpdateInfo info);
};