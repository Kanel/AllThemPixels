#pragma once

#include <SFML\Graphics.hpp>

using namespace sf;

class GlareEffect
{
public:
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

public:
	GlareEffect();

	void draw(RenderWindow * window);
};