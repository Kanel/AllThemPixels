#include "HexagonGrid.h"

int neighbors[6][2];

Vector3f HexagonGrid::convertToCubeCoordinates(Vector2f axialCoordinates)
{
	float x = axialCoordinates.x;
	float z = axialCoordinates.y;
	float y = -x - z;

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

HexagonGrid::HexagonGrid(Hexagon::Style style, float size)
{
	this->style = style;
	this->size = size;
	this->layer = 0;
	this->segment = 6;
	this->segmentPosition = 0;
	this->nextCoordinate = AxialCoordinates();

	neighbors[Up]		 = AxialCoordinates(0, -1);
	neighbors[UpRight]	 = AxialCoordinates(1, -1);
	neighbors[DownRight] = AxialCoordinates(1, 0);
	neighbors[Down]		 = AxialCoordinates(0, 1);
	neighbors[DownLeft]  = AxialCoordinates(-1, 1);
	neighbors[UpLeft]	 = AxialCoordinates(-1, 0);
}

int HexagonGrid::getNumberOfTiles(int layers)
{
	return (layers == 0) ? 1 : getNumberOfTiles(layers - 1) + layers * 6;
}

int HexagonGrid::getNumberOfTilesInLayer(int layer)
{
	return (layer > 0) ? layer * 6 : 1;
}

Vector2f HexagonGrid::getPosition(AxialCoordinates axialCoordinates)
{
	float x;
	float y;

	if (style == Hexagon::FlatTopped)
	{
		x = size * (3.0f / 2.0f) * axialCoordinates.q;
		y = size * sqrt(3) * (axialCoordinates.r + (axialCoordinates.q / 2.0f));
	}
	else
	{
		x = size * sqrt(3) * (axialCoordinates.q + axialCoordinates.r / 2.0f);
		y = size * (3.0f / 2.0f) * axialCoordinates.r;
	}
	return Vector2f(x, y);
}

AxialCoordinates HexagonGrid::getAxialCoordinates(Vector2f position)
{
	float q;
	float r;
	Vector3f cubeCoordiantes;

	if (style == Hexagon::FlatTopped)
	{
		q = position.x / (size * (3.0f / 2.0f));
		r = position.y / (size * sqrt(3)) - ((position.x / (size * (3.0f / 2.0f))) / 2.0f);
	}
	else
	{
		q = position.x / (size * sqrt(3)) - ((position.y / (size * (3.0f / 2.0f))) / 2.0f);
		r = position.y / (size * (3.0f / 2.0f));	
	}
	cubeCoordiantes = convertToCubeCoordinates(Vector2f(q, r));

	return convertToAxialCoordinates(hexRound(cubeCoordiantes));
}

AxialCoordinates HexagonGrid::step(AxialCoordinates axialCoordinates, HexagonDirection direction)
{
	return axialCoordinates + neighbors[direction]; 
}

void HexagonGrid::setOrigin(AxialCoordinates origin)
{
	this->origin = origin;
}

AxialCoordinates HexagonGrid::next()
{
	AxialCoordinates current = nextCoordinate;

	nextCoordinate = step(nextCoordinate, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + segment) % 6));
	segmentPosition++;

	if (segmentPosition >= layer) // Check if the step was into a another segment.
	{
		segment++;
		segmentPosition = 0;
	}
	if (segment >= 6) // Check if the segment has already been visited.
	{
		layer++;
		segment = 0;
		segmentPosition = 0;
		nextCoordinate = AxialCoordinates(-layer, layer); // Change layer.
	}
	return current + origin;
}

AxialCoordinates HexagonGrid::next(int &layer)
{
	AxialCoordinates current;

	layer = this->layer;
	current = next();

	return current;
}

AxialCoordinates HexagonGrid::next(Vector2f &position)
{
	AxialCoordinates current = next();

	position = getPosition(current);

	return current;
}

vector<AxialCoordinates> HexagonGrid::getCornerRegionCoordinates(int corner)
{
	return vector<AxialCoordinates>();
}

vector<AxialCoordinates> HexagonGrid::getRingCoordinates(int layer)
{
	int index = 0;
	vector<AxialCoordinates> coordinates(getNumberOfTilesInLayer(layer));
	AxialCoordinates hexagon = AxialCoordinates(-layer, layer);

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < layer; j++)
		{
			coordinates[index++] = hexagon;

			hexagon = step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
		}
	}
	return coordinates;
}

Hexagon *** HexagonGrid::generateGrid(Vector2f position, int layers, VertexCollection * vertexSource)
{
	int spacing = 0;
	int matrixLength = (layers * 2) + 1;
	Vector2i offset(layers, layers);
	Hexagon *** matrix;
	Vector2f hexagonPosition = position + getPosition(AxialCoordinates(0, 0));
	
	// Allocate matrix.
	matrix = new Hexagon**[matrixLength];

	for (int i = 0; i < matrixLength; i++)
	{
		matrix[i] = new Hexagon*[matrixLength];

		for (int j = 0; j < matrixLength; j++)
		{
			matrix[i][j] = NULL;
		}
	}

	// Populate matrix.
	matrix[offset.x][offset.y] = new Hexagon(hexagonPosition, size - spacing, Color(255, 255, 255), style, vertexSource);

	for (int k = 1; k <= layers; k++)
	{
		AxialCoordinates hexagon = AxialCoordinates(-k, k);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < k; j++)
			{
				hexagonPosition = position + getPosition(hexagon);
				matrix[offset.x + hexagon.q][offset.y + hexagon.r] = new Hexagon(hexagonPosition, size - spacing, Color(255, 255, 255), style, vertexSource);
				
				hexagon = step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
			}
		}
	}
	
	return matrix;
}