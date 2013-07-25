#pragma once

#include "UpdateInfo.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class Effect : public Drawable, public Transformable
{
private:


public:
	virtual bool isExpended() = 0;
	virtual void update(UpdateInfo info) = 0;
	virtual void draw(RenderTarget& target, RenderStates states) const = 0;
};