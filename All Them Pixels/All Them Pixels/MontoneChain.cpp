#include "MonotoneChain.h"

bool MonotoneChain::compare(Vector2f a, Vector2f b)
{
	return (a.x < b.x) || (a.x == b.x && a.y < b.y);
}

float MonotoneChain::cross(std::vector<Vector2f> verticies, Vector2f next)
{
	Vector2f o = verticies[verticies.size() - 2];
	Vector2f a = verticies[verticies.size() - 1];
	Vector2f b = next;

	return Vector2fMath::cross(a - o, b - o);
}

void MonotoneChain::clean(std::vector<Vector2f> &verticies, Vector2f next)
{
	while (verticies.size() >= 2 && cross(verticies, next) <= 0)
	{			
		verticies.pop_back();			
	}
}

ConvexHull MonotoneChain::getConvexHull(std::vector<Vector2f> verticies)
{
	std::vector<Vector2f> upper;
	std::vector<Vector2f> lower;

	std::sort(verticies.begin(), verticies.end(), compare);

	for (int i = 0; i < verticies.size(); i++)
	{
		clean(lower, verticies[i]);
		lower.push_back(verticies[i]);
	}

	for (int i = verticies.size() - 1; i >= 0; i--)
	{
		clean(upper, verticies[i]);
		upper.push_back(verticies[i]);
	}

	upper.pop_back();
	lower.pop_back();
	lower.insert(lower.end(), upper.begin(), upper.end());

	return lower;
}