#include "AI.h"

Vector2f AI::aproximation(Vector2f vector)
{
	return Vector2f(floorf(vector.x*10000)/10000, floorf(vector.y*10000)/10000);
}

AIProperties AI::generate(int difficulty)
{
	AIProperties aiProperties;

	aiProperties.aimavoidance = (difficulty > 3) ? 50 + rand() % 50 : 0;
	aiProperties.playerspace = (float)(rand() % 200) / 100.0;
	aiProperties.righthanded = (rand() % 2 == 0);
	aiProperties.speed = 1+(float)(rand() % (difficulty * 200)) / 100.0;

	return aiProperties;
}

void AI::update(queue<Entity *> *spawnQueue, Enemy * target, Player * player, UpdateInfo info)
{
	int shots;
	Weapon weapon = target->weapon;
	AIProperties properties = target->aiProperties;
	float projectileSpeed = weapon.config.speed;
	float speed = properties.speed;
	int range = 100;// weapon->ttl * speed; //make correct
	Vector2f direction = Vector2fMath::unitVector(player->getPosition() - target->getPosition());

	// Determine in which direction to circle the player.
	if (Vector2fMath::distance(player->getPosition(), target->getPosition()) < range * properties.playerspace)
	{
		direction = properties.righthanded ? Vector2fMath::turn90right(direction) : Vector2fMath::turn90left(direction) ;
	}

	// Dodge behaviour.
	if (properties.aimavoidance > 0)
	{
		Vector2f a = player->getPosition();
		Vector2f p = target->getPosition();
		Vector2f n = player->aimVector;
		Vector2f v1 = Vector2fMath::dot((a-p), n)*n;
		Vector2f v2 = aproximation(Vector2fMath::invert(Vector2fMath::unitVector(v1)));
		Vector2f v3 = aproximation(n);
		float distance = Vector2fMath::length((a-p)-v1);

		if (distance < properties.aimavoidance && v3 == v2)
		{
			direction = Vector2fMath::invert(Vector2fMath::unitVector((a-p)-v1));
		}
	}

	// Move the controlled enemy.
	target->translate(direction * speed);

	// Fire the weapon as many times as allowed.
	if (weapon.isReady(info.elapsedGameTime, info.updateInterval, shots))
	{
		for (int i = 0; i < shots; i++)
		{
			spawnQueue->push(weapon.fire(target->getPosition(), Vector2fMath::unitVector(player->getPosition() - target->getPosition()), info.elapsedGameTime, EntityTypes::EnemyProjectileEntity));
		}
	}
}