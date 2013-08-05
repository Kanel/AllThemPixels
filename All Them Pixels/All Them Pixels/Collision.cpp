#include "Collision.h"

Rect<float> Collision::transformRectangle(Rect<float> rectangle, Vector2f speed)
{
	if (speed.x < 0)
	{
		rectangle.left += speed.x;
		rectangle.width -= speed.x;
	}
	else
	{
		rectangle.width += speed.x;
	}

	if (speed.y < 0)
	{
		rectangle.top += speed.y;
		rectangle.height -= speed.y;
	}
	else
	{
		rectangle.height += speed.y;
	}
	assert(rectangle.width >= 0 && rectangle.height >= 0);

	return rectangle;
}

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

bool Collision::hitBoxesOverlap(Rect<float> a, Rect<float> b)
{
	if (a.top > b.top + b.height) return false;
	if (a.top + a.height < b.top) return false;
	if (a.left + a.width < b.left) return false;
	if (a.left > b.left + b.width) return false;

	return true;
}

bool Collision::hitBoxesOverlap(Rect<float> a, Vector2f speedA, Rect<float> b, Vector2f speedB)
{
	a = transformRectangle(a, speedA);
	b = transformRectangle(b, speedB);

	return hitBoxesOverlap(a, b);
}

bool Collision::hitBoxesOverlap(Rect<float> a, Rect<float> b, Vector2f speedB)
{
	b = transformRectangle(b, speedB);

	return hitBoxesOverlap(a, b);
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

/* maybe
take torritory borders and entity position and some threshold, return vector for translation.
maybe usefull for enemies and player, projectiles?
Vector2f Collision::borderControl()
{
	Vector2f center = position;

	Vector2f playerCenter = player->getPosition();
	playerCenter.x += 10; //fix
	playerCenter.y += 10; //fix

	int quadrant;

	if (playerCenter.x < center.x)
		if (playerCenter.y < center.y)
			quadrant = 4;
		else
			quadrant = 3;
	else
		if (playerCenter.y < center.y)
			quadrant = 1;
		else
			quadrant = 2;

	//imagine player position in quadrant four.
	switch (quadrant)
	{
	case 1:
		playerCenter.x = playerCenter.x - 2*(playerCenter.x - center.x);
		break;
	case 2:
		playerCenter.x = playerCenter.x - 2*(playerCenter.x - center.x);
		playerCenter.y = playerCenter.y - 2*(playerCenter.y - center.y);
		break;
	case 3:
		playerCenter.y = playerCenter.y - 2*(playerCenter.y - center.y);
		break;
	case 4:
		break;
	default:
		break;
	}
	/*
		Vector2f a = corner position;
		Vector2f p = player center;
		Vector2f n = unit vector pointing to next corner or down;
		Vector2f v1 = Vector2fMath::dot((a-p), n)*n;

		float distance = Vector2fMath::length((a-p)-v1);
		if distance is less than some radius of player:
			player->translate
		if in safe and distance is smaller.
			change territory

}
*/