#include "AI.h"

void AI::update(Territory * territory, Enemy * target, Player * player, UpdateInfo info)
{
	float projectileSpeed = 4;
	float speed = 2;
	int range = 100;
	Vector2f direction = Vector2fMath::unitVector(player->getPosition() - target->getPosition());

	if (Vector2fMath::distance(player->getPosition(), target->getPosition()) < range * 2)
	{
		float t = direction.x;
		direction.x = direction.y;
		direction.y = -t;
	}

	target->translate(direction * speed);

	if (target->getLastShootFired() + target->getCooldown() <= info.elapsedGameTime)
	{
		target->setLastShootFired(info.elapsedGameTime);
		territory->addEntity(new Projectile(target->getPosition(), direction * projectileSpeed, 20.0, range, Color::Blue, EnemyProjectileEntity));
	}
}