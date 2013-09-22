#pragma once

#include "UpdateInfo.h"
#include <SFML\Graphics.hpp>

using namespace sf;

/**
 * Abstract base class for classes that cause movement.
 */
class Movement
{
protected:
	Vector2f * position; /**< The position which movement is inflected onto. */

public:
	/**
     * Sets the position which should be manipulated.
	 * @param position The position to inflict movement onto.
	 */
	virtual void setPosition(Vector2f * position);

	/**
	 * Causes movement.
	 * @param info Information about the update.
	 */
	virtual void update(UpdateInfo info) = 0;
};