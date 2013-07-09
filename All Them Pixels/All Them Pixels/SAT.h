// SAT - Separating Axis Theorem

#pragma once

#include "Vector2fMath.h"
#include <list>
#include <math.h>
#include <SFML/Graphics.hpp>

using namespace sf;

typedef std::vector<Vector2f> ConvexHull;

static class SAT
{
private:
	static std::vector<Vector2f> getVectors(std::vector<Vector2f> vertices);
	static void getMinMax(std::vector<Vector2f> vertices, Vector2f axis, float &min, float &max);
	static Vector2f getPenetrationVector(float polygonAMin, float polygonAMax, float polygonBMin, float polygonBMax, float projectedMovement, Vector2f axis, float axisMagnitude);
	static bool isSeparated(std::vector<Vector2f> polygonAVertices, std::vector<Vector2f> polygonBVertices, Vector2f movement, std::list<Vector2f> &penetration, bool isReference);

public:
	static Vector2f getShortestPenetration(std::list<Vector2f> penetration);
	static bool collides(ConvexHull polygonAVertices, ConvexHull polygonBVertices, std::list<Vector2f> &penetration);
	static bool collides(ConvexHull polygonAVertices, ConvexHull polygonBVertices, Vector2f movement, std::list<Vector2f> &penetration);
	static bool collides(std::list<ConvexHull> shapeA, std::list<ConvexHull> shapeB, std::list<Vector2f> &penetration);
	static bool collides(std::list<ConvexHull> shapeA, std::list<ConvexHull> shapeB, Vector2f movement, std::list<Vector2f> &penetration);
};