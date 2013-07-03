#pragma once

#include "Entity.h"
#include <SFML\Graphics.hpp>
#include <SFML\Window\Joystick.hpp>

using namespace sf;

class GlareEffect : public Entity
{
private:
	float hwd;
	float hwod;
	float hwid;
	float ochd;
	float ocvd;
	float ichd;
	float icvd;
	float vwod;
	float vwid;
	Vertex o;	
	Vertex owclt;
	Vertex howlt;
	Vertex howlb;
	Vertex owclb;
	Vertex iwclt;
	Vertex hiwlt;
	Vertex hiwlb;
	Vertex iwclb;
	Vertex owcrt;
	Vertex howrt;
	Vertex howrb;
	Vertex owcrb;
	Vertex iwcrt;
	Vertex hiwrt;
	Vertex hiwrb;
	Vertex iwcrb;
	Vertex vwot;
	Vertex vwob;
	Vertex vwit;
	Vertex vwib;
	VertexArray innerShape;
	VertexArray outerShape;
	VertexArray circle;

	Vertex aimVertices[2];

protected:
	void applyTransform(Transform transform, VertexArray &vertices);

public:
	GlareEffect(Vector2f position);

	bool isEtheral() override;

	Rect<float> getBoundingBox() override;

	void applyTransform(Transform transform) override;
	void draw(RenderWindow * window) override;
	void update(UpdateInfo info) override;
};