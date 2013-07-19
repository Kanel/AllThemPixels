#pragma once

#define _USE_MATH_DEFINES

#include <math.h>
#include <SFML\Graphics.hpp>

using namespace sf;

class CircleBar : public Transformable, public Drawable
{
private:
	int sides;
	float radius;
	float width;
	float borderWidth;
	float value;
	float maximumValue;
	Vertex * border;
	Vertex * bar;

private:
	void update();

public:
	CircleBar(int sides, float radius, float width, float borderWidth, float maximumValue, float value = 0);

	float getValue();
	void setValue(float value);

	virtual void draw(RenderTarget& target, RenderStates states) const;
};