#include "FloorTiles.h"
#include "Shapes.h"

Vector3f FloorTiles::convertToCubeCoordinates(Vector2f axialCoordinates)
{
	int x = axialCoordinates.x;
	int z = axialCoordinates.y;
	int y = -x - z;

	return Vector3f(x, y, z);
}

Vector2i FloorTiles::convertToAxialCoordinates(Vector3i cubeCoordinates)
{
	return Vector2i(cubeCoordinates.x, cubeCoordinates.z);
}
Vector3i FloorTiles::hexRound(Vector3f cubeCoordiantes)
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

/*Vector2i getHexagonAtPosition(float x, float y, float size)
{
	float q = x / (size * 3.0f/2.0f);
	float r = y / (size * sqrt(3)) - (x / (size * 3.0f/2.0f)) / 2.0f;
	Vector3f cubeCoordiantes = getCubeCoordinates(Vector2f(q, r));
	Vector3i actualCubeCoordinates = hexRound(cubeCoordiantes);

	return getAxialCoordinates(actualCubeCoordinates);
}*/

Vector2f FloorTiles::getHexagonPosition(int q, int r, float size)
{
	return Vector2f(size * (3.0f/2.0f) * q, size * sqrt(3) * (r + (q/2.0f)));
}

int FloorTiles::getNumberOfTiles(int layers)
{
	return (layers == 1) ? 1 : getNumberOfTiles(layers - 1) + (layers - 1) * 6;
}

FloorTiles::FloorTiles() { }

FloorTiles::FloorTiles(Vector2i windowSize, Vector2f position, Color colors[3], int layers, int tileSize)
{
	int line, column, offset;
	float width = sqrt(3 * (tileSize * tileSize) / 4);
	int neighbors[6][2] = { {+1, 0}, {+1, -1}, {0, -1}, {-1, 0}, {-1, +1}, {0, +1} };

	tiles.resize(getNumberOfTiles(layers));

	this->windowSize = windowSize;
	tileCount = 0;

	// Generate center hexagon.
	Shapes::hexagon(tiles[tileCount++].corners, 0, position, tileSize, colors[tileCount % 3]);

	// Walk through every hexagon one layer at a time.
	for (int k = 1; k < layers; k++)
	{
		Vector2i hexagon(-k, k);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < k; j++)
			{
				Shapes::hexagon(tiles[tileCount++].corners, 0, position + getHexagonPosition(hexagon.x, hexagon.y, tileSize), tileSize, colors[k % 3]);

				hexagon = Vector2i(hexagon.x + neighbors[i][0], hexagon.y + neighbors[i][1]);
			}
		}
	}
}

void FloorTiles::draw(sf::RenderWindow * window)
{
	for (int i = 0; i < tileCount; i++)
	{
		window->draw(tiles[i].corners, 6, PrimitiveType::TrianglesFan);
	}
}