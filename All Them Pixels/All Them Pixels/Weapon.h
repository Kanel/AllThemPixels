#pragma once

#include "Projectile.h"
#include "VertexCollection.h"
#include <SFML/Graphics.hpp>

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
	VertexCollection * vertexSource;
	int lastFired; // game time

public:
	Weapon();
	Weapon(WeaponConfiguration config, VertexCollection * vertexSource);

	void setConfiguration(WeaponConfiguration config);

	Projectile * fire(Vector2f position, Vector2f direction, int gameTime, EntityTypes type);
	bool isReady(int gameTime, int updateInterval, int &shots);
};