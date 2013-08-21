#include "ValueBar.h"

ValueBar::ValueBar(Vector2f start, Vector2f end, float margin, float size, float min, float max, float value)
{
	this->start = start;
	this->end = end;
	this->margin = margin;
	this->size = size;
	this->min = min;
	this->max = max;
	this->value = value;

	generateBackground();
	generateForeground();
}

void ValueBar::setPoints(Vector2f start, Vector2f end)
{
	this->start = start;
	this->end = end;

	generateBackground();
	generateForeground();
}

float ValueBar::getValue()
{
	return value;
}

ConvexHull ValueBar::getConvexHull()
{
	ConvexHull hull(4);

	hull[0] = background[0].position;
	hull[1] = background[1].position;
	hull[2] = background[2].position;
	hull[3] = background[3].position;

	return hull;
}

bool ValueBar::containsPoint(Vector2f point)
{
	if (Collision::containsPoint(background[0].position, background[1].position, background[2].position, point) ||
		Collision::containsPoint(background[2].position, background[3].position, background[0].position, point))
	{
		return true;
	}
}

void ValueBar::update(Vector2f point)
{
	if (containsPoint(point))
	{
		Vector2f bar = end - start;
		Vector2f progress = point - start;
	
		value = min + (max - min) * (Vector2fMath::length(progress) / Vector2fMath::length(bar));
	}
	generateBackground();
	generateForeground();
}

void ValueBar::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(background, 4, PrimitiveType::Quads, states);
	target.draw(foreground, 4, PrimitiveType::Quads, states);
}

void ValueBar::generateBackground()
{
	Vector2f direction = Vector2fMath::unitVector(end - start);
	Vector2f left = Vector2fMath::leftNormal(direction);
	Vector2f right = Vector2fMath::rightNormal(direction);

	background[0] = Vertex(Vector2f(start.x + left.x * size, start.y + left.y * size), Color(0, 0, 0, 200));
	background[1] = Vertex(Vector2f(start.x + right.x * size, start.y + right.y * size), Color(0, 0, 0, 200));
	background[2] = Vertex(Vector2f(end.x + right.x * size, end.y + right.y * size), Color(0, 0, 0, 200));
	background[3] = Vertex(Vector2f(end.x + left.x * size, end.y + left.y * size), Color(0, 0, 0, 200));
}

void ValueBar::generateForeground()
{
	Vector2f direction = Vector2fMath::unitVector(end - start);
	Vector2f left = Vector2fMath::leftNormal(direction);
	Vector2f right = Vector2fMath::rightNormal(direction);
	Vector2f end = start + (this->end - start) * ((value - min) / (max - min));

	foreground[0] = Vertex(Vector2f(start.x + left.x * (size - margin), start.y + left.y * (size - margin)), Color(255, 255, 255, 200));
	foreground[1] = Vertex(Vector2f(start.x + right.x * (size - margin), start.y + right.y * (size - margin)), Color(255, 255, 255, 200));
	foreground[2] = Vertex(Vector2f(end.x + right.x * (size - margin), end.y + right.y * (size - margin)), Color(255, 255, 255, 200));
	foreground[3] = Vertex(Vector2f(end.x + left.x * (size - margin), end.y + left.y * (size - margin)), Color(255, 255, 255, 200));
}