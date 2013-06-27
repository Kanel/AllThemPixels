#include "Hexagon.h"

Hexagon::Hexagon() { }

Hexagon::Hexagon(Vector2f position, float radius, Color color, Style style)
{
	if (style == Style::FlatTopped)
	{
		// Flat topped
		float width = radius * 2;
		float halfWidth = radius;
		float quarterWidth = radius / 2;
		float height = sqrt(3)/2 * width;
		float halfHeight = height / 2;

		corners[0] = Vertex(Vector2f(position.x - quarterWidth, position.y - halfHeight), color);
		corners[1] = Vertex(Vector2f(position.x + quarterWidth, position.y - halfHeight), color);
		corners[2] = Vertex(Vector2f(position.x + halfWidth, position.y), color);
		corners[3] = Vertex(Vector2f(position.x + quarterWidth, position.y + halfHeight), color);
		corners[4] = Vertex(Vector2f(position.x - quarterWidth, position.y + halfHeight), color);
		corners[5] = Vertex(Vector2f(position.x - halfWidth, position.y), color);

		boundingBox.left = position.x - halfWidth;
		boundingBox.top = position.y - halfHeight;
		boundingBox.width = width;
		boundingBox.height = height;		
	}
	else
	{
		// Pointy topped
		float quarterHeight = radius / 2;
		float halfHeight = radius;
		float height = radius * 2;
		float width = sqrtf(3) / 2 * height;
		float halfWidth = width / 2;

		corners[0] = Vertex(Vector2f(position.x, position.y - halfHeight), color);
		corners[1] = Vertex(Vector2f(position.x + halfWidth, position.y - quarterHeight), color);
		corners[2] = Vertex(Vector2f(position.x + halfWidth, position.y + quarterHeight), color);
		corners[3] = Vertex(Vector2f(position.x, position.y + halfHeight), color);
		corners[4] = Vertex(Vector2f(position.x - halfWidth, position.y + quarterHeight), color);
		corners[5] = Vertex(Vector2f(position.x - halfWidth, position.y - quarterHeight), color);
	
		boundingBox.left = position.x - halfWidth;
		boundingBox.top = position.y - halfHeight;
		boundingBox.width = width;
		boundingBox.height = height;
	}
	convexHull.resize(6);

	convexHull[0] = corners[0].position;
	convexHull[1] = corners[1].position;
	convexHull[2] = corners[2].position;
	convexHull[3] = corners[3].position;
	convexHull[4] = corners[4].position;
	convexHull[5] = corners[5].position;
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

void Hexagon::setColor(Color color)
{
	corners[0].color = color;
	corners[1].color = color;
	corners[2].color = color;
	corners[3].color = color;
	corners[4].color = color;
	corners[5].color = color;
}

void Hexagon::draw(RenderWindow * window)
{
	window->draw(corners, 6, PrimitiveType::TrianglesFan);
}