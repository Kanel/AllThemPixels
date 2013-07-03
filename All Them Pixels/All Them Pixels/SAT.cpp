#include "SAT.h"
// The vertices should be ordered clockwise? (Does it matter?)

std::vector<Vector2f> SAT::getVectors(std::vector<Vector2f> vertices)
{
	int index = 0;
	std::vector<Vector2f> vectors(vertices.size());

	for (int i = 0; i < vertices.size() - 1; i++)
	{
		Vector2f a = vertices[i];
		Vector2f b = vertices[i + 1];
		
		vectors[index++] = Vector2fMath::leftNormal(b - a);
	}
	vectors[index] = Vector2fMath::leftNormal(vertices[0] - vertices[vertices.size() - 1]);

	return vectors;
}

void SAT::getMinMax(std::vector<Vector2f> vertices, Vector2f axis, float &min, float &max)
{
	min = Vector2fMath::dot(vertices[0], axis);
	max = min;

	for (int i = 1; i < vertices.size(); i++)
	{
		float projectedValue = Vector2fMath::dot(vertices[i], axis);

		min = (min > projectedValue) ? projectedValue : min;
		max = (max < projectedValue) ? projectedValue : max;
	}
}

Vector2f SAT::getPenetrationVector(float polygonAMin, float polygonAMax, float polygonBMin, float polygonBMax, float projectedMovement, Vector2f axis, float axisMagnitude)
{
	float a = polygonBMax - (polygonAMin + projectedMovement);
	float b = (polygonAMax + projectedMovement) - polygonBMin;
	float penetrationMagnitude = (abs(a) < abs(b)) ? a : b;
	Vector2f penetration = axis;	

	if (polygonAMin < polygonBMin)
	{
		penetrationMagnitude = -penetrationMagnitude;
	}

	penetration.x *= penetrationMagnitude;
	penetration.y *= penetrationMagnitude;

	return penetration;
}

bool SAT::isSeparated(std::vector<Vector2f> polygonAVertices, std::vector<Vector2f> polygonBVertices, Vector2f movement, std::list<Vector2f> &penetration, bool isReference)
{
	std::vector<Vector2f> polygonAVectors = getVectors(polygonAVertices);

	for (int i = 0; i < polygonAVectors.size(); i++)
	{		
		float polygonAMin;
		float polygonAMax;
		float polygonBMin;
		float polygonBMax;
		float magnitude = sqrtf(powf(polygonAVectors[i].x, 2) + powf(polygonAVectors[i].y, 2));
		Vector2f axis(polygonAVectors[i].x / magnitude, polygonAVectors[i].y / magnitude);
		float projectedMovement = Vector2fMath::dot(movement, axis);

		getMinMax(polygonAVertices, axis, polygonAMin, polygonAMax);
		getMinMax(polygonBVertices, axis, polygonBMin, polygonBMax);

		if (polygonBMin >= polygonAMax + projectedMovement || polygonAMin + projectedMovement >= polygonBMax)
		{
			penetration.clear();

			break;
		}
		else
		{
			Vector2f translation = getPenetrationVector(polygonAMin, polygonAMax, polygonBMin, polygonBMax, projectedMovement, axis, magnitude);

			if (!isReference)
			{
				translation = Vector2fMath::invert(translation);
			}

			penetration.push_back(translation);
		}
	}
	return penetration.size() == 0;
}

Vector2f SAT::getShortestPenetration(std::list<Vector2f> penetration)
{
	Vector2f shortest = penetration.front();

	for (std::list<Vector2f>::iterator it = penetration.begin()++; it != penetration.end(); it++)
	{
		if (Vector2fMath::length(*it) < Vector2fMath::length(shortest))
		{
			shortest = *it;
		}
	}
	return shortest;
}

bool SAT::collides(std::vector<Vector2f> polygonAVertices, std::vector<Vector2f> polygonBVertices,  std::list<Vector2f> &penetration)
{
	return collides(polygonAVertices, polygonBVertices, Vector2f(), penetration);
}

bool SAT::collides(std::vector<Vector2f> polygonAVertices, std::vector<Vector2f> polygonBVertices, Vector2f movement,  std::list<Vector2f> &penetration)
{	
	bool a = !isSeparated(polygonAVertices, polygonBVertices, movement, penetration, true);
	bool b = !isSeparated(polygonBVertices, polygonAVertices, movement, penetration, false);

	return a && b;
}

bool SAT::collides(std::list<ConvexHull> shapeA, std::list<ConvexHull> shapeB, std::list<Vector2f> &penetration)
{
	return collides(shapeA, shapeB, Vector2f(), penetration);
}

// Bah
bool SAT::collides(std::list<ConvexHull> shapeA, std::list<ConvexHull> shapeB, Vector2f movement,  std::list<Vector2f> &penetration)
{	
	bool isCollission = false;
	std::list<Vector2f> solution;

	for (std::list<ConvexHull>::iterator itA = shapeA.begin(); itA != shapeA.end(); itA++)
	{
		for (std::list<ConvexHull>::iterator itB = shapeB.begin(); itB != shapeB.end(); itB++)
		{	
			bool a = !isSeparated(*itA, *itB, movement, solution, true);
			bool b = !isSeparated(*itB, *itA, movement, solution, false);

			if (a && b)
			{
				isCollission = true;

				penetration.push_back(getShortestPenetration(solution));				
			}
			solution.clear();
		}
	}
	return isCollission;
}