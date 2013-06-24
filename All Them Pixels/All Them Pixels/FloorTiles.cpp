#include "FloorTiles.h"
#include "Shapes.h"

FloorTiles::FloorTiles()
{

}

Vector3f getCubeCoordinates(Vector2f axialCoordinates)
{
	int x = axialCoordinates.x;
	int z = axialCoordinates.y;
	int y = -x - z;

	return Vector3f(x, y, z);
}

Vector2i getAxialCoordinates(Vector3i cubeCoordinates)
{
	return Vector2i(cubeCoordinates.x, cubeCoordinates.z);
}

int round(float f)
{
	// Todo: figure out what type of round is meant to happen here...
	 return floor(f + 0.5f);
}

Vector3i hexRound(Vector3f cubeCoordiantes)
{
    int rx = round(cubeCoordiantes.x);
    int ry = round(cubeCoordiantes.y);
    int rz = round(cubeCoordiantes.z);

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

Vector2i getHexagonAtPosition(float x, float y, float size)
{
	float q = x / (size * 3.0f/2.0f);
	float r = y / (size * sqrt(3)) - (x / (size * 3.0f/2.0f)) / 2.0f;
	Vector3f cubeCoordiantes = getCubeCoordinates(Vector2f(q, r));
	Vector3i actualCubeCoordinates = hexRound(cubeCoordiantes);

	return getAxialCoordinates(actualCubeCoordinates);
}

Vector2f getHexagonPosition(int q, int r, float size)
{
	return Vector2f(size * (3.0f/2.0f) * q, size * sqrt(3) * (r + (q/2.0f)));
}

FloorTiles::FloorTiles(Vector2i windowSize, Vector2f position, Color colors[3], int layers, int tileSize)
{
	this->windowSize = windowSize;
	int line, column, offset;
	float width = sqrt(3 * (tileSize * tileSize) / 4);
	// direction

	int neighbors[6][2] = { {+1, 0}, {+1, -1}, {0, -1}, {-1, 0}, {-1, +1}, {0, +1} };

	tiles.resize(8000);

	tileCount = 0;

	Shapes::hexagon(tiles[tileCount++].corners, 0, position, tileSize, colors[tileCount % 3]);

	for (int k = 1; k < layers; k++)
	{
		Vector2i H(-k, k);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < k; j++)
			{
				Vector2f v = getHexagonPosition(H.x, H.y, tileSize);

				Shapes::hexagon(tiles[tileCount++].corners, 0, position + getHexagonPosition(H.x, H.y, tileSize), tileSize, colors[k % 3]);

				H = Vector2i(H.x + neighbors[i][0], H.y + neighbors[i][1]);
			}
		}
	}

	/*
	tiles.resize(100);

	for (int i = 0; i < 100; i++)
	{
		line = int(i / 10);
		column = i % 10;
		offset = (line % 2) ? width : 0;

		Shapes::hexagon(tiles[i].corners, 0, Vector2f(2 * column * width + offset, line * (tileSize + tileSize / 2)), tileSize, colors[(2 * (line % 2) + column % 2)]);
	}*/

	


}

FloorTiles::~FloorTiles()
{

}

void FloorTiles::move(Vector2f movement)
{
	float width = sqrt(3 * (200 * 200) / 4);

	float x = tiles[0].corners[0].position.x;
	float y = tiles[0].corners[0].position.y;

	if (x + movement.x < -200)
		movement.x += 2 * width;
	if (x + movement.x > 0)
		movement.x -= 2 * width;
	if (y + movement.y < -300)
		movement.y += 300;
	if (y + movement.y > -100)
		movement.y -= 300;

	for (int i = 0; i < tileCount; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			tiles[i].corners[j].position.x += movement.x;
			tiles[i].corners[j].position.y += movement.y;
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