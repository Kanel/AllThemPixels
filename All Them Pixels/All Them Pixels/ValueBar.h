#pragma once

#include "Vector2fMath.h"
#include "Collision.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class ValueBar : public Drawable
{
private:
	float min;
	float max;
	float value;
	float size;
	float margin;
	Vector2f start;
	Vector2f end;
	Vertex background[4];
	Vertex foreground[4];

public:
	ValueBar(Vector2f start, Vector2f end, float margin, float size, float min = 0, float max = 100, float value = 0);
	
	void setPoints(Vector2f start, Vector2f end);
	float getValue();

	void update(Vector2f position);
	
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	void generateBackground();
	void generateForeground();
};