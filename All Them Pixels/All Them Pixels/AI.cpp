#include "AI.h"

void AI::update(Territory * territory, Enemy * target, Player * player, UpdateInfo info)
{
	float projectileSpeed = 4;
	float speed = 2;
	int range = 50;
	Vector2f direction = Vector2fMath::unitVector(player->getPosition() - target->getPosition());

	if (Vector2fMath::distance(player->getPosition(), target->getPosition()) < range)
	{
		float t = direction.x;
		direction.x = direction.y;
		direction.y = -t;
	}

	target->translate(direction * speed);

	target->cooldownThisShouldNotBePublic -= info.elapsedSeconds;

	if (target->cooldownThisShouldNotBePublic <= 0)
	{
		target->cooldownThisShouldNotBePublic = 2000;
		territory->addEntity(new Projectile(target->getPosition(), direction * projectileSpeed, 20.0, range, Color::Blue, EnemyProjectileEntity));
	}
}