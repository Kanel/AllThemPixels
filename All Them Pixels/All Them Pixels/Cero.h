#pragma once

#define _USE_MATH_DEFINES

#include "UpdateInfo.h"
#include "AlphaShape.h"
#include <math.h>
#include <list>
#include <SFML\Graphics.hpp>

using std::list;
using namespace sf;

struct CeroArms
{
	list<Vertex> particles;

};

class Cero : public Transformable, public Drawable
{
private:
	int particleCount;
	int radius;
	int centerRadius;
	int centerSides;
	Vector2f position;
	Vertex ** particles;
	Vertex * centerCircle;
	float rotation;
	float spiralBend;
	int spirals;
	int spiralSegments;
	float spiralLength;
	int activeSegemnts;

public:
	Cero(Vector2f position, int radius);
	~Cero();

	void update(UpdateInfo info);

	virtual void draw(RenderTarget& target, RenderStates states) const;
};