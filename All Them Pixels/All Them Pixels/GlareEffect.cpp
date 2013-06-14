#include "GlareEffect.h"

GlareEffect::GlareEffect()
{	
	Vector2f position(250, 250);
	Color core(255, 0, 0, 255);
	Color inner(255, 0, 0, 175);
	Color outer(255, 0, 0, 50);

	hwd = 1;
	hwod = 200;
	hwid = 10;
	ochd = 4;
	ocvd = 3;
	ichd = 1;
	icvd = 1;
	vwod = 40;
	vwid = 20;
	
	o = Vertex(position, core);
	
	owclt = Vertex(Vector2f(position.x + -ochd, position.y + ocvd), outer);
	howlt = Vertex(Vector2f(position.x + -hwod, position.y + hwd), outer);
	howlb = Vertex(Vector2f(position.x + -hwod, position.y + -hwd), outer);
	owclb = Vertex(Vector2f(position.x + -ochd, position.y + -ocvd), outer);

	iwclt = Vertex(Vector2f(position.x + -ichd, position.y + icvd), inner);
	hiwlt = Vertex(Vector2f(position.x + -hwid, position.y + hwd), inner);
	hiwlb = Vertex(Vector2f(position.x + -hwid, position.y + -hwd), inner);
	iwclb = Vertex(Vector2f(position.x + -ichd, position.y + -icvd), inner);

	owcrt = Vertex(Vector2f(position.x + ochd, position.y + ocvd), outer);
	howrt = Vertex(Vector2f(position.x + hwod, position.y + hwd), outer);
	howrb = Vertex(Vector2f(position.x + hwod, position.y + -hwd), outer);
	owcrb = Vertex(Vector2f(position.x + ochd, position.y + -ocvd), outer);

	iwcrt = Vertex(Vector2f(position.x + ichd, position.y + icvd), inner);
	hiwrt = Vertex(Vector2f(position.x + hwid, position.y + hwd), inner);
	hiwrb = Vertex(Vector2f(position.x + hwid, position.y + -hwd), inner);
	iwcrb = Vertex(Vector2f(position.x + ichd, position.y + -icvd), inner);

	vwot = Vertex(Vector2f(position.x, position.y + vwod), outer);
	vwob = Vertex(Vector2f(position.x, position.y + -vwod), outer);

	vwit = Vertex(Vector2f(position.x, position.y + vwid), inner);
	vwib = Vertex(Vector2f(position.x, position.y + -vwid), inner);

	innerShape.setPrimitiveType(PrimitiveType::TrianglesFan);
	innerShape.append(o);
	innerShape.append(vwit);
	innerShape.append(iwcrt);
	innerShape.append(hiwrt);
	innerShape.append(hiwrb);
	innerShape.append(iwcrb);
	innerShape.append(vwib);
	innerShape.append(iwclb);
	innerShape.append(hiwlb);
	innerShape.append(hiwlt);
	innerShape.append(iwclt);
	innerShape.append(vwit);

	outerShape.setPrimitiveType(PrimitiveType::TrianglesStrip);
	outerShape.append(vwob);
	outerShape.append(vwib);
	outerShape.append(owclb);
	outerShape.append(iwclb);
	outerShape.append(howlb);
	outerShape.append(hiwlb);
	outerShape.append(howlt);
	outerShape.append(hiwlt);
	outerShape.append(owclt);
	outerShape.append(iwclt);
	outerShape.append(vwot);
	outerShape.append(vwit);
	outerShape.append(owcrt);
	outerShape.append(iwcrt);
	outerShape.append(howrt);
	outerShape.append(hiwrt);
	outerShape.append(howrb);
	outerShape.append(hiwrb);
	outerShape.append(owcrb);
	outerShape.append(iwcrb);
	outerShape.append(vwob);
	outerShape.append(vwib);
}
void GlareEffect::draw(RenderWindow * window)
{
	window->draw(innerShape);
	window->draw(outerShape);
}