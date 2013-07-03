#pragma once

#include "Projectile.h"
#include <SFML\Graphics.hpp>

struct WeaponConfiguration
{
	int cooldown; // milliseconds
	int spread; // degrees from center aim
	int damage;
	int piercing;
	float speed;
	int ttl;
};

class Weapon
{
public:
	WeaponConfiguration config;
	int lastFired; // game time

public:
	Weapon(WeaponConfiguration config);

	Projectile * fire(Vector2f position, Vector2f direction, int gameTime);
	bool isReady(int gameTime);
};