#include "GlareEffect.h"

void GlareEffect::applyTransform(Transform transform, VertexArray &vertices)
{
	for (int i = 0; i < vertices.getVertexCount(); i++)
	{
		vertices[i].position = transform.transformPoint(vertices[i].position);
	}
}

GlareEffect::GlareEffect(Vector2f position) : GlareEffect::Entity(position)
{
	Color core(255, 150, 150, 255);
	Color inner(255, 0, 0, 170);
	Color outer(255, 0, 0, 50);

	hwd = 1;
	hwod = 200;
	hwid = 60;
	ochd = 1;
	ocvd = 1;
	ichd = 1;
	icvd = 1;
	vwod = 20;
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

	int x = position.x;
	int y = position.y;
	int size = 22;
	int width = 3;
	int vertices = 128;
	float angleOffset = 6.28318530718 / vertices;
	Vector2f * outerLayer = new Vector2f[vertices];
	Vector2f * innerLayer = new Vector2f[vertices];

	for (int i = 0; i < vertices; i++)
	{
		outerLayer[i].x = x + (cosf(angleOffset * i) * size);
		outerLayer[i].y = y + (sinf(angleOffset * i) * size);
	}

	for (int i = 0; i < vertices; i++)
	{
		innerLayer[i].x = x + (cosf(angleOffset * i) * (size - width));
		innerLayer[i].y = y + (sinf(angleOffset * i) * (size - width));
	}

	circle.setPrimitiveType(PrimitiveType::TrianglesStrip);

	for (int i = 0; i < vertices; i++)
	{
		circle.append(Vertex(Vector2f(innerLayer[i].x, innerLayer[i].y), inner));
		circle.append(Vertex(Vector2f(outerLayer[i].x, outerLayer[i].y), outer));
	}
	circle.append(Vertex(Vector2f(innerLayer[0].x, innerLayer[0].y), inner));
	circle.append(Vertex(Vector2f(outerLayer[0].x, outerLayer[0].y), outer));

	delete[] outerLayer;
	delete[] innerLayer;

	aimVertices[0] = Vertex(Vector2f(position.x,position.y));
	aimVertices[1] = Vertex(Vector2f(position.x,position.y));
}

bool GlareEffect::isEtheral()
{
	return true;
}

Rect<float> GlareEffect::getBoundingBox()
{
	return Rect<float>(position.x - 10, position.y - 10, 20, 20);
}

void GlareEffect::applyTransform(Transform transform)
{
	Entity::applyTransform(transform);

	applyTransform(transform, circle);
	applyTransform(transform, innerShape);
	applyTransform(transform, outerShape);
}

void GlareEffect::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(circle);
	target.draw(innerShape);
	target.draw(outerShape);
	
	target.draw(aimVertices, 2, PrimitiveType::Lines);

}

void GlareEffect::update(UpdateInfo info, Controls * controls)
{

}