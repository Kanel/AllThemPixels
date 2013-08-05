#pragma once

#include "Entity.h"
#include <cassert>

using std::vector;

static class Collision
{
private:
	static Rect<float> transformRectangle(Rect<float> rectangle, Vector2f speed);

public:
	static Rect<float> getHitBox(Vertex vertices[], int count);
	static ConvexHull getConvexHull(Vertex vertices[], int count);
	static bool hitBoxesOverlap(Rect<float> a, Rect<float> b);
	static bool hitBoxesOverlap(Rect<float> a, Vector2f speedA, Rect<float> b, Vector2f speedB);
	static bool hitBoxesOverlap(Rect<float> a, Rect<float> b, Vector2f speedB);
	static bool isClose(Entity * entity1, Entity * entity2);
	static bool isWithinWindow(Rect<float> boundingbox, View view);
	static bool containsPoint(Rect<float> boundingBox, Vector2f point);
};