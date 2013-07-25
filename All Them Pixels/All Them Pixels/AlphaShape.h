#pragma once

#include "DelaunayTessalation.h"


static class AlphaShape
{
public:
	static Vertex *  getTriangles(Vertex * vertices, int count, float radius, int &triangleCount);
};