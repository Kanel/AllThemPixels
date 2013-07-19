#include "CircleBar.h"

void CircleBar::update()
{
	float reservedAngle = (2 * M_PI) * 0.10f;
	float angle = reservedAngle / 2;
	float stepAngle = (((2 * M_PI) - reservedAngle) * (value / maximumValue)) / (sides - 1); // Den tar 3 steg framåt inte 4
	float innerBorder = radius - borderWidth;
	float outerborder = radius + width + borderWidth;
	float innerBar = radius;
	float outerBar = radius + width;
	int index = 0;
	Vertex * bar = new Vertex[2 * sides];	
	Vertex * border = new Vertex[2 * sides];

	for(int i = 0; i < sides; i++, index += 2, angle += stepAngle)
	{
		Vector2f unit(cosf(angle), sinf(angle));

		bar[index] = Vertex(unit * innerBar, Color(0, 0, 0, 255));
		bar[index + 1] = Vertex(unit * outerBar, Color(0, 0, 0, 255));
		border[index] = Vertex(unit * innerBorder, Color(255, 255, 255, 255));
		border[index + 1] = Vertex(unit * outerborder, Color(255, 255, 255, 255));
	}

	delete this->bar;
	delete this->border;

	this->bar = bar;
	this->border = border;
}

CircleBar::CircleBar(int sides, float radius, float width, float borderWidth, float maximumValue, float value)
{
	this->sides = sides;
	this->radius = radius;
	this->width = width;
	this->borderWidth = borderWidth;
	this->maximumValue = maximumValue;
	this->value = value;
	this->bar = new Vertex[0];
	this->border = new Vertex[0];

	update();
}

float CircleBar::getValue()
{
	return value;
}

void CircleBar::setValue(float value)
{
	this->value = (value > maximumValue) ? maximumValue : value;

	update();
}

void CircleBar::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(border, sides * 2, PrimitiveType::TrianglesStrip, states);	
	target.draw(bar, sides * 2, PrimitiveType::TrianglesStrip, states);	
}