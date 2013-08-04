#pragma once

#define _USE_MATH_DEFINES

#include "Entity.h"
#include "Shapes.h"
#include "HexagonGridStorage.h"
#include "AxialCoordinates.h"
#include <math.h>
#include <vector>

using std::vector;

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
	AxialCoordinates origin;
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
	HexagonGrid(Hexagon::Style style, float size = 0);
	//HexagonGrid(Hexagon::Style style, float size, AxialCoordinates origin);

	static int getNumberOfTiles(int layers);
	static int getNumberOfTilesInLayer(int layer);

	Vector2f getPosition(AxialCoordinates axialCoordinates);
	AxialCoordinates getAxialCoordinates(Vector2f position);
	AxialCoordinates step(AxialCoordinates axialCoordinates, HexagonDirection direction);

	void setOrigin(int q, int r);
	void setOrigin(AxialCoordinates origin);

	void reset();

	AxialCoordinates next();
	AxialCoordinates next(int &layer);
	AxialCoordinates next(Vector2f &position);

	vector<AxialCoordinates> getCornerRegionCoordinates(int corner);
	vector<AxialCoordinates> getRingCoordinates(int layer);
	HexagonGridStorage generateGrid(Vector2f position, int layers, VertexCollection * vertexSource);
};