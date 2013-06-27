#pragma once

#define _USE_MATH_DEFINES

#include "Entity.h"
#include "Shapes.h"
#include <math.h>

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

enum HexagonStyle
{
	FlatTopped,
	PointyTopped
};

class HexagonGrid
{
public:
	AxialCoordinates neighbors[6];
	HexagonStyle style;

private:
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
	HexagonGrid(HexagonStyle style);

	int getNumberOfTiles(int layers);
	int getNumberOfTilesInLayer(int layer);

	Vector2f getPosition(AxialCoordinates axialCoordinates, float size);
	AxialCoordinates getAxialCoordinates(Vector2f position, float size);
	AxialCoordinates step(AxialCoordinates axialCoordinates, HexagonDirection direction);
};