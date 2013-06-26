#include "AI.h"

void AI::update(Territory * territory, Enemy * target, Player * player, UpdateInfo info)
{
	float projectileSpeed = 4;
	float speed = 2;
	int ttl = 50;
	Vector2f direction = Vector2fMath::unitVector(player->getPosition() - target->getPosition());

	target->translate(direction * speed);

	target->cooldownThisShouldNotBePublic -= info.elapsedSeconds;

	if (target->cooldownThisShouldNotBePublic <= 0)
	{
		target->cooldownThisShouldNotBePublic = 2000;
	//	territory->addEntity(new Projectile(target->getPosition(), direction * projectileSpeed, ttl));
	}
}