#include "AlphaShape.h"

Vertex * AlphaShape::getTriangles(Vertex * vertices, int count, float radius, int &triangleCount)
{
	int index = 0;
	int tessalatedTriangeCount;
	float maxDistance = radius * 2;
	Vertex * tessalation = DelaunayTessalation::getTriangles(vertices, count, tessalatedTriangeCount);
	Vertex * alphaShape = new Vertex[tessalatedTriangeCount * 3];

	triangleCount = 0;

	for (int i = 0; i < tessalatedTriangeCount * 3; i += 3)
	{
		float a;
		float b;
		float c;
		Vertex * A = &tessalation[i + 0];
		Vertex * B = &tessalation[i + 1];
		Vertex * C = &tessalation[i + 2];

		a = Vector2fMath::distance(A->position, B->position);
		b = Vector2fMath::distance(B->position, C->position);
		c = Vector2fMath::distance(C->position, A->position);

		if (a <= maxDistance && b <= maxDistance && c <= maxDistance)
		{
			alphaShape[index++] = tessalation[i + 0];
			alphaShape[index++] = tessalation[i + 1];
			alphaShape[index++] = tessalation[i + 2];
			triangleCount++;
		}
	}
	delete[] tessalation;

	return alphaShape;
}