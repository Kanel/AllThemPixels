#include "Collision.h"

bool Collision::isClose(Entity * entity1, Entity * entity2)
{
	Rect<float> hitbox1 = entity1->getBoundingBox(); 
	Rect<float> hitbox2 = entity2->getBoundingBox(); 

	if (hitbox1.top > hitbox2.top + hitbox2.height) return false; //entity1 is below entity2
	if (hitbox1.top + hitbox1.height < hitbox2.top) return false; //entity1 is above entity2
	if (hitbox1.left + hitbox1.width < hitbox2.left) return false; //entity1 is to the left of entity2
	if (hitbox1.left > hitbox2.left + hitbox2.width) return false; //entity1 is to the right of entity2
	return true; // entity1 is preeetty close to entity 2
	             //                     ^ and how close it that exactly?
}