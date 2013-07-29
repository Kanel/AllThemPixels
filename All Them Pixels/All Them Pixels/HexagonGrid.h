#pragma once

#define _USE_MATH_DEFINES

#include "Entity.h"
#include "Shapes.h"
#include "Hexagon.h"
#include <math.h>
#include <vector>

using std::vector;

class AxialCoordinates
{
public:
	int q;
	int r;

public:
	AxialCoordinates() 
	{
		this->q = 0;
		this->r = 0;
	}

	AxialCoordinates(int q, int r)
	{
		this->q = q;
		this->r = r;
	}

	AxialCoordinates operator+(AxialCoordinates &other)
	{
		return AxialCoordinates(q + other.q, r + other.r);
	}
};

class HexagonGrid
{
public:
	AxialCoordinates neighbors[6];
	Hexagon::Style style;

private:
	int layer;
	int segment;
	int segmentPosition;
	float size;
	AxialCoordinates nextCoordinate;
	Vector3f convertToCubeCoordinates(Vector2f axialCoordinates);
	AxialCoordinates convertToAxialCoordinates(Vector3i cubeCoordinates);
	Vector3i hexRound(Vector3f cubeCoordiantes);

public:
	enum HexagonDirection 
	{
		Up = 1,
		UpRight = 0,
		DownRight = 5,
		Down = 4,
		DownLeft = 3,
		UpLeft = 2
	};

public:
	HexagonGrid(Hexagon::Style style, float size);
	//HexagonGrid(Hexagon::Style style, float size, AxialCoordinates origin);

	int getNumberOfTiles(int layers);
	int getNumberOfTilesInLayer(int layer);

	Vector2f getPosition(AxialCoordinates axialCoordinates);
	AxialCoordinates getAxialCoordinates(Vector2f position);
	AxialCoordinates step(AxialCoordinates axialCoordinates, HexagonDirection direction);

	AxialCoordinates next();
	AxialCoordinates next(Vector2f &position);

	vector<AxialCoordinates> getCornerRegionCoordinates(int corner);
	vector<AxialCoordinates> getRingCoordinates(int layer);
	Hexagon *** generateGrid(Vector2f position, int layers, VertexCollection * vertexSource);
};