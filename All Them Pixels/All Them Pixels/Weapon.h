#pragma once

#include "Projectile.h"
#include <SFML\Graphics.hpp>

class Weapon
{
public:
	int cooldown; // milliseconds
	int spread; // degrees from center line
	int damage;
	int piercing;
	int lastFired; // game time
	float speed;
	int ttl;

public:
	Weapon(int cooldown, int spread, int damage, int piercing, float speed, int ttl);

	Projectile * fire(Vector2f position, Vector2f direction, int gameTime);
	bool isReady(int gameTime);
};