#include "Collision.h"

Rect<float> Collision::getHitBox(Vertex vertices[], int count)
{
	float minX = vertices[0].position.x;
	float maxX = vertices[0].position.x;
	float minY = vertices[0].position.y;
	float maxY = vertices[0].position.y;

	for (int i = 1; i < count; i++)
	{
		if (vertices[i].position.x < minX)
		{
			minX = vertices[i].position.x;
		}
		else if (maxX < vertices[i].position.x)
		{
			maxX = vertices[i].position.x;
		}

		if (vertices[i].position.y < minY)
		{
			minY = vertices[i].position.y;
		}
		else if (maxY < vertices[i].position.y)
		{
			maxY = vertices[i].position.y;
		}
	}
	return Rect<float>(minX, minY, maxX - minX, maxY - minY);
}

ConvexHull Collision::getConvexHull(Vertex vertices[], int count)
{
	vector<Vector2f> positions(count);

	for (int i = 0; i < count; i++)
	{
		positions[i] = vertices[i].position;
	}

	return MonotoneChain::getConvexHull(positions);
}

bool Collision::hitBoxesOverlap(Rect<float> hitbox1, Rect<float> hitbox2)
{
	if (hitbox1.top > hitbox2.top + hitbox2.height) return false;
	if (hitbox1.top + hitbox1.height < hitbox2.top) return false;
	if (hitbox1.left + hitbox1.width < hitbox2.left) return false;
	if (hitbox1.left > hitbox2.left + hitbox2.width) return false;
	return true;
}

bool Collision::isClose(Entity * entity1, Entity * entity2)
{
	Rect<float> hitbox1 = entity1->getBoundingBox(); 
	Rect<float> hitbox2 = entity2->getBoundingBox(); 

	return hitBoxesOverlap(hitbox1, hitbox2);
}

bool Collision::isWithinWindow(Rect<float> hitbox1, View view)
{
	Rect<float> hitbox2;

	hitbox2.height = view.getSize().y;
	hitbox2.width = view.getSize().x;
	hitbox2.top = view.getCenter().y - hitbox2.height / 2;
	hitbox2.left = view.getCenter().x - hitbox2.width / 2;

	return hitBoxesOverlap(hitbox1, hitbox2);
}

bool Collision::containsPoint(Rect<float> boundingBox, Vector2f point)
{
	return boundingBox.left <= point.x && point.x <= boundingBox.left + boundingBox.width &&
		   boundingBox.top <= point.y  && point.y <= boundingBox.top + boundingBox.height;
}