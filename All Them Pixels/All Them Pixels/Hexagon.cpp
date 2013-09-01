#include "Hexagon.h"

Hexagon::Hexagon(Vector2f position, float radius, Color color, Style style, VertexCollection * vertexSource)
{
	//this->position = position;
	this->radius = radius;
	this->vertexSource = vertexSource;

	vertexOffset = vertexSource->add();

	//colorStack.push(color);

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
	return Collision::getHitBox(&(*vertexSource)[vertexOffset], 6);
}

ConvexHull Hexagon::getConvexHull()
{
	ConvexHull convexHull(6);

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
	return convexHull;
}

Vector2f * Hexagon::getPoints(int &count)
{
	Vector2f * points;

	count = 6;
	points = new Vector2f[count];

	points[0] = (*vertexSource)[vertexOffset + 1].position;
	points[1] = (*vertexSource)[vertexOffset + 2].position;
	points[2] = (*vertexSource)[vertexOffset + 3].position;
	points[3] = (*vertexSource)[vertexOffset + 4].position;
	points[4] = (*vertexSource)[vertexOffset + 5].position;
	points[5] = (*vertexSource)[vertexOffset + 6].position;

	return points;
}

Color Hexagon::getColor()
{
	return (*vertexSource)[vertexOffset].color;
}

void Hexagon::setColor(Color color)
{
	Vertex * vertices = &(*vertexSource)[vertexOffset];

	if ((*vertexSource)[vertexOffset].color != color)
	{
		vertices[0].color = color;
		vertices[1].color = color;
		vertices[2].color = color;
		vertices[3].color = color;
		vertices[4].color = color;
		vertices[5].color = color;
		vertices[6].color = color;
		vertices[7].color = color;
	}
}

void Hexagon::morph(float length, float width)
{

		(*vertexSource)[vertexOffset + 4].position.x += length;
		(*vertexSource)[vertexOffset + 5].position.x += length;
		(*vertexSource)[vertexOffset + 6].position.x += length;
		(*vertexSource)[vertexOffset + 7].position.x += length;
		
		(*vertexSource)[vertexOffset + 2].position.y += width/2;
		(*vertexSource)[vertexOffset + 3].position.y -= width/2;
		(*vertexSource)[vertexOffset + 4].position.y += width/2;
		(*vertexSource)[vertexOffset + 5].position.y -= width/2;

}

void Hexagon::applyTransform(Transform &transform)
{
	Vertex * vertices = &(*vertexSource)[vertexOffset];

	vertices[0].position = transform.transformPoint(vertices[0].position);
	vertices[1].position = vertices[0].position;
	vertices[2].position = transform.transformPoint(vertices[2].position);
	vertices[3].position = transform.transformPoint(vertices[3].position);
	vertices[4].position = transform.transformPoint(vertices[4].position);
	vertices[5].position = transform.transformPoint(vertices[5].position);
	vertices[6].position = transform.transformPoint(vertices[6].position);
	vertices[7].position = vertices[6].position;
}