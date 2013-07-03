#include "Weapon.h"

Weapon::Weapon(WeaponConfiguration config)
{
	this->config = config;
	this->lastFired = 0;
}

Projectile * Weapon::fire(Vector2f position, Vector2f direction, int gameTime)
{
	Transform transform;
	Vector2f speedVector = direction * config.speed;
	Color color;
	unsigned int colorComposite = 0;

	colorComposite += config.cooldown;
	colorComposite += config.spread;
	colorComposite += config.damage;
	colorComposite += config.piercing;
	colorComposite += config.speed;
	colorComposite += config.ttl;

	color.r = (colorComposite & 0x000000FF) >> 0;
	color.g = (colorComposite & 0x0000FF00) >> 8;
	color.b = (colorComposite & 0x00FF0000) >> 16;

	transform.rotate(config.spread - rand() % (config.spread * 2));

	speedVector = transform.transformPoint(speedVector);

	lastFired = gameTime;

	return new Projectile(position, speedVector, config.damage, config.ttl, color);
}

bool Weapon::isReady(int gameTime)
{
	return lastFired + config.cooldown <= gameTime;
}