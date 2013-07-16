#include "FloorTile.h"

FloorTile::FloorTile(Vector2f position, float radius, Color color, VertexCollection * vertexSource) : hexagon(position, radius, color, Hexagon::FlatTopped, vertexSource)
{
	originalColor = color;
}

Color FloorTile::getColor()
{
	return currentColor;
}

void FloorTile::setColor(Color color)
{
	currentColor = color;

	hexagon.setColor(color);
}

void FloorTile::resetColor()
{
	if (originalColor != currentColor)
	{
		setColor(originalColor);
	}
}

void FloorTile::fadeToOriginalColor(int amount)
{
	if (amount == 0)
	{
		resetColor();
	}
	else
	{
		Color currentColor = getColor();
		Color newColor = Color(originalColor.r - ((amount - 1) * (float)(originalColor.r - currentColor.r) / amount), //detta kan omöjligen förenklas!!
							   originalColor.g - ((amount - 1) * (float)(originalColor.g - currentColor.g) / amount), 
							   originalColor.b - ((amount - 1) * (float)(originalColor.b - currentColor.b) / amount));

		setColor(newColor);
	}
}

Rect<float> FloorTile::getBoundingBox()
{
	return hexagon.getBoundingBox();
}

ConvexHull FloorTile::getConvexHull()
{
	return hexagon.getConvexHull();
}

void FloorTile::update(UpdateInfo info)
{

}

FloorTile *** FloorTile::generateGrid(Vector2f position, float hexagonRadius, int layers, VertexCollection * vertexSource)
{
	int spacing = 0;
	int matrixLength = (layers * 2) + 1;
	HexagonGrid grid(Hexagon::FlatTopped);
	Vector2i offset(layers, layers);
	FloorTile *** matrix;
	Vector2f hexagonPosition = position + grid.getPosition(AxialCoordinates(0, 0), hexagonRadius);
	
	// Allocate matrix.
	matrix = new FloorTile**[matrixLength];

	for (int i = 0; i < matrixLength; i++)
	{
		matrix[i] = new FloorTile*[matrixLength];

		for (int j = 0; j < matrixLength; j++)
		{
			matrix[i][j] = NULL;
		}
	}

	// Populate matrix.
	matrix[offset.x][offset.y] = new FloorTile(hexagonPosition, hexagonRadius - spacing, Color(255, 255, 255), vertexSource);

	for (int k = 1; k <= layers; k++)
	{
		AxialCoordinates hexagon = AxialCoordinates(-k, k);

		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < k; j++)
			{
				hexagonPosition = position + grid.getPosition(hexagon, hexagonRadius);
				int grayness = rand() % 30;
				matrix[offset.x + hexagon.q][offset.y + hexagon.r] = new FloorTile(hexagonPosition, hexagonRadius - spacing, Color(255-grayness, 255-grayness, 255-grayness), vertexSource);
				
				hexagon = grid.step(hexagon, (HexagonGrid::HexagonDirection)((HexagonGrid::DownRight + i) % 6));
			}
		}
	}
	
	return matrix;
}