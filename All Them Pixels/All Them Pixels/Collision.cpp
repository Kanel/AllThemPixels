#include "Collision.h"

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

bool Collision::isWithinWindow(Rect<float> hitbox1, RenderWindow * window)
{
	Rect<float> hitbox2;
	hitbox2.height = window->getView().getSize().y;
	hitbox2.width = window->getView().getSize().x;
	hitbox2.top = window->getView().getCenter().y - hitbox2.height / 2;
	hitbox2.left = window->getView().getCenter().x - hitbox2.width / 2;
	hitbox2.width -= 100; //for demonstration

	return hitBoxesOverlap(hitbox1, hitbox2);
}