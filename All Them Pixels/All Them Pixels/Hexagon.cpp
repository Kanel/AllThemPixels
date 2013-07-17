#include "Hexagon.h"

//Hexagon::Hexagon() { }

Hexagon::Hexagon(Vector2f position, float radius, Color color, Style style, VertexCollection * vertexSource) : convexHull(6)
{
	this->originalColor = color;
	this->vertexSource = vertexSource;

	vertexCount = 8;
	vertexOffset = vertexSource->add();

	if (style == FlatTopped)
	{
		// Flat topped
		float width = radius * 2;
		float halfWidth = radius;
		float quarterWidth = radius / 2;
		float height = sqrt(3)/2 * width;
		float halfHeight = height / 2;

		(*vertexSource)[vertexOffset + 0] = Vertex(Vector2f(position.x - halfWidth, position.y), color);
		(*vertexSource)[vertexOffset + 1] = Vertex(Vector2f(position.x - halfWidth, position.y), color);
		(*vertexSource)[vertexOffset + 2] = Vertex(Vector2f(position.x - quarterWidth, position.y + halfHeight), color);
		(*vertexSource)[vertexOffset + 3] = Vertex(Vector2f(position.x - quarterWidth, position.y - halfHeight), color);
		(*vertexSource)[vertexOffset + 4] = Vertex(Vector2f(position.x + quarterWidth, position.y + halfHeight), color);
		(*vertexSource)[vertexOffset + 5] = Vertex(Vector2f(position.x + quarterWidth, position.y - halfHeight), color);
		(*vertexSource)[vertexOffset + 6] = Vertex(Vector2f(position.x + halfWidth, position.y), color);
		(*vertexSource)[vertexOffset + 7] = Vertex(Vector2f(position.x + halfWidth, position.y), color);
		
		boundingBox.left = position.x - halfWidth;
		boundingBox.top = position.y - halfHeight;
		boundingBox.width = width;
		boundingBox.height = height;

		convexHull[0] = (*vertexSource)[vertexOffset + 1].position;
		convexHull[1] = (*vertexSource)[vertexOffset + 3].position;
		convexHull[2] = (*vertexSource)[vertexOffset + 5].position;
		convexHull[3] = (*vertexSource)[vertexOffset + 6].position;
		convexHull[4] = (*vertexSource)[vertexOffset + 4].position;
		convexHull[5] = (*vertexSource)[vertexOffset + 2].position;
	}
	else
	{
		// Pointy topped
		float quarterHeight = radius / 2;
		float halfHeight = radius;
		float height = radius * 2;
		float width = sqrtf(3) / 2 * height;
		float halfWidth = width / 2;

		(*vertexSource)[vertexOffset + 0] = Vertex(Vector2f(position.x, position.y - halfHeight), color);
		(*vertexSource)[vertexOffset + 1] = Vertex(Vector2f(position.x, position.y - halfHeight), color);
		(*vertexSource)[vertexOffset + 2] = Vertex(Vector2f(position.x - halfWidth, position.y - quarterHeight), color);
		(*vertexSource)[vertexOffset + 3] = Vertex(Vector2f(position.x + halfWidth, position.y - quarterHeight), color);
		(*vertexSource)[vertexOffset + 4] = Vertex(Vector2f(position.x - halfWidth, position.y + quarterHeight), color);
		(*vertexSource)[vertexOffset + 5] = Vertex(Vector2f(position.x + halfWidth, position.y + quarterHeight), color);
		(*vertexSource)[vertexOffset + 6] = Vertex(Vector2f(position.x, position.y + halfHeight), color);
		(*vertexSource)[vertexOffset + 7] = Vertex(Vector2f(position.x, position.y + halfHeight), color);
	
		boundingBox.left = position.x - halfWidth;
		boundingBox.top = position.y - halfHeight;
		boundingBox.width = width;
		boundingBox.height = height;

		convexHull[0] = (*vertexSource)[vertexOffset + 2].position;
		convexHull[1] = (*vertexSource)[vertexOffset + 1].position;
		convexHull[2] = (*vertexSource)[vertexOffset + 3].position;
		convexHull[3] = (*vertexSource)[vertexOffset + 5].position;
		convexHull[4] = (*vertexSource)[vertexOffset + 6].position;
		convexHull[5] = (*vertexSource)[vertexOffset + 4].position;
	}
}

Hexagon::~Hexagon()
{
	vertexSource->remove(vertexOffset);
}

Hexagon::Style Hexagon::getStyle()
{
	return style;
}

Rect<float> Hexagon::getBoundingBox()
{
	return boundingBox;
}

ConvexHull Hexagon::getConvexHull()
{
	return convexHull;
}

Color Hexagon::getColor()
{
	return (*vertexSource)[vertexOffset].color;
}

void Hexagon::setColor(Color color)
{
	for (int i = 0; i < vertexCount; i++)
	{
		(*vertexSource)[vertexOffset + i].color = color;
	}
}

void Hexagon::applyTransform(Transform transform)
{
	for (int i = 0; i < vertexCount; i++)
	{
		(*vertexSource)[vertexOffset + i].position = transform.transformPoint((*vertexSource)[vertexOffset + i].position);
	}

	if (style == FlatTopped)
	{
		convexHull[0] = (*vertexSource)[vertexOffset + 1].position;
		convexHull[1] = (*vertexSource)[vertexOffset + 3].position;
		convexHull[2] = (*vertexSource)[vertexOffset + 5].position;
		convexHull[3] = (*vertexSource)[vertexOffset + 6].position;
		convexHull[4] = (*vertexSource)[vertexOffset + 4].position;
		convexHull[5] = (*vertexSource)[vertexOffset + 2].position;
	}
	else
	{
		convexHull[0] = (*vertexSource)[vertexOffset + 2].position;
		convexHull[1] = (*vertexSource)[vertexOffset + 1].position;
		convexHull[2] = (*vertexSource)[vertexOffset + 3].position;
		convexHull[3] = (*vertexSource)[vertexOffset + 5].position;
		convexHull[4] = (*vertexSource)[vertexOffset + 6].position;
		convexHull[5] = (*vertexSource)[vertexOffset + 4].position;
	}
	boundingBox = Collision::getHitBox(&(*vertexSource)[vertexOffset], 8);
}