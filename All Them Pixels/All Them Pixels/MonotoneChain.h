#pragma once

#include "Vector2fMath.h"
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>

using namespace sf;

typedef std::vector<Vector2f> ConvexHull;

class MonotoneChain
{
private:
	static bool compare(Vector2f a, Vector2f b);
	static float cross(std::vector<Vector2f> verticies, Vector2f next);
	static void clean(std::vector<Vector2f> &verticies, Vector2f next);

public:
	static ConvexHull getConvexHull(std::vector<Vector2f> verticies);
};