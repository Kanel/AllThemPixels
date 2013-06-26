#include "HexagonGrid.h"

int neighbors[6][2];

Vector3f HexagonGrid::convertToCubeCoordinates(AxialCoordinates axialCoordinates)
{
	int x = axialCoordinates.q;
	int z = axialCoordinates.r;
	int y = -x - z;

	return Vector3f(x, y, z);
}

AxialCoordinates HexagonGrid::convertToAxialCoordinates(Vector3i cubeCoordinates)
{
	return AxialCoordinates(cubeCoordinates.x, cubeCoordinates.z);
}

Vector3i HexagonGrid::hexRound(Vector3f cubeCoordiantes)
{
	int rx = floor(cubeCoordiantes.x + 0.5f);
    int ry = floor(cubeCoordiantes.y + 0.5f);
    int rz = floor(cubeCoordiantes.z + 0.5f);

    float x_err = abs(rx - cubeCoordiantes.x);
    float y_err = abs(ry - cubeCoordiantes.y);
    float z_err = abs(rz - cubeCoordiantes.z);
		
    if (x_err > y_err && x_err > z_err)
	{
        rx = -ry - rz;
	}
    else if (y_err > z_err)
	{
        ry = -rx - rz;
	}
    else
	{
        rz = -rx - ry;
	}

	return Vector3i(rx, ry, rz);
}

HexagonGrid::HexagonGrid()
{
	neighbors[Up]		 = AxialCoordinates(0, -1);
	neighbors[UpRight]	 = AxialCoordinates(1, -1);
	neighbors[DownRight] = AxialCoordinates(1, 0);
	neighbors[Down]		 = AxialCoordinates(0, 1);
	neighbors[DownLeft]  = AxialCoordinates(-1, 1);
	neighbors[UpLeft]	 = AxialCoordinates(-1, 0);
}

int HexagonGrid::getNumberOfTiles(int layers)
{
	return (layers == 1) ? 1 : getNumberOfTiles(layers - 1) + (layers - 1) * 6;
}

int HexagonGrid::getNumberOfTilesInLayer(int layer)
{
	return (layer > 1) ? (layer - 1) * 6 : 1;
}

Vector2f HexagonGrid::getPosition(AxialCoordinates axialCoordinates, float size)
{
	return Vector2f(size * (3.0f / 2.0f) * axialCoordinates.q, size * sqrt(3) * (axialCoordinates.r + (axialCoordinates.q / 2.0f)));
}

AxialCoordinates HexagonGrid::getAxialCoordinates(Vector2f position, float size)
{
	float q = position.x / (size * 3.0f/2.0f);
	float r = position.y / (size * sqrt(3)) - (position.x / (size * 3.0f/2.0f)) / 2.0f;
	Vector3f cubeCoordiantes = convertToCubeCoordinates(AxialCoordinates(q, r));
	Vector3i actualCubeCoordinates = hexRound(cubeCoordiantes);

	return convertToAxialCoordinates(actualCubeCoordinates);
}

AxialCoordinates HexagonGrid::step(AxialCoordinates axialCoordinates, HexagonDirection direction)
{
	return axialCoordinates + neighbors[direction]; 
}