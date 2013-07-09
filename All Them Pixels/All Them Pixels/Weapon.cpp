#include "Weapon.h"

Weapon::Weapon() { }

Weapon::Weapon(WeaponConfiguration config, VertexCollection * vertexSource)
{
	this->config = config;
	this->vertexSource= vertexSource;
	this->lastFired = 0;
}

void Weapon::setConfiguration(WeaponConfiguration config)
{
	this->config = config;
}

Projectile * Weapon::fire(Vector2f position, Vector2f direction, int gameTime, EntityTypes type)
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

	return new Projectile(position, speedVector, config.damage, config.ttl, vertexSource, color, type);
}

bool Weapon::isReady(int gameTime, int updateInterval, int &shots)
{
	bool ready = lastFired + config.cooldown <= gameTime;

	if (lastFired > 0)
	{
		bool gr�t = true;
	}

	if (config.cooldown < updateInterval)
	{
		shots = ((float)updateInterval / (float)config.cooldown) + 0.5f;
	}
	else if (ready)
	{
		shots = 1;
	}


	return ready;
}