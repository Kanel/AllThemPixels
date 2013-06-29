#pragma once

#include "Entity.h"

static class Collision
{
public:
	static bool hitBoxesOverlap(Rect<float> hitbox1, Rect<float> hitbox2);
	static bool isClose(Entity * entity1, Entity * entity2);
	static bool isWithinWindow(Rect<float> boundingbox, View view);
};