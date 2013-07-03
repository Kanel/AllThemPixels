#pragma once

#include "Entity.h"

using std::vector;

static class Collision
{
public:
	static Rect<float> getHitBox(Vertex vertices[], int count);
	static ConvexHull getConvexHull(Vertex vertices[], int count);
	static bool hitBoxesOverlap(Rect<float> hitbox1, Rect<float> hitbox2);
	static bool isClose(Entity * entity1, Entity * entity2);
	static bool isWithinWindow(Rect<float> boundingbox, View view);
	static bool containsPoint(Rect<float> boundingBox, Vector2f point);
};