#include "Weapon.h"

Weapon::Weapon(int cooldown, int spread, int damage, int piercing, float speed, int ttl)
{
	this->cooldown = cooldown;
	this->spread = spread;
	this->damage = damage;
	this->piercing = piercing;
	this->speed = speed;
	this->lastFired = 0;
	this->ttl = ttl;
}

Projectile * Weapon::fire(Vector2f position, Vector2f direction, int gameTime)
{
	Transform transform;
	Vector2f speedVector = direction * speed;
	Color color;
	unsigned int colorComposite = 0;

	colorComposite += cooldown;
	colorComposite += spread;
	colorComposite += damage;
	colorComposite += piercing;
	colorComposite += speed;
	colorComposite += ttl;

	color.r = (colorComposite & 0x000000FF) >> 0;
	color.g = (colorComposite & 0x0000FF00) >> 8;
	color.b = (colorComposite & 0x00FF0000) >> 16;

	transform.rotate(spread - rand() % (spread * 2));

	speedVector = transform.transformPoint(speedVector);

	lastFired = gameTime;

	return new Projectile(position, speedVector, damage, ttl, color);
}

bool Weapon::isReady(int gameTime)
{
	return lastFired + cooldown <= gameTime;
}