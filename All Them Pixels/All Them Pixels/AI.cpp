#include "AI.h"

Vector2f AI::aproximation(Vector2f vector)
{
	return Vector2f(floorf(vector.x*10000)/10000, floorf(vector.y*10000)/10000);
}

AIProperties AI::generate(int difficulty)
{
	AIProperties aiProperties;

	assert(difficulty > 0);

	aiProperties.aimAvoidanceDistance = (difficulty > 3) ? randomInt(50, 100) : 0;
	aiProperties.aimAvoidanceSpeed = (difficulty > 3) ? randomInt(0, difficulty * 10) : 0;
	aiProperties.movementSpeed = randomInt(0, difficulty * 100);
	aiProperties.orbitalSpeed = randomInt(-100, 100);
	aiProperties.orbitalThreshold = randomFloat(10, 200, 2);
	aiProperties.orbitalDistance = randomFloat(50, 500, 2);
	aiProperties.stalkingDistance = aiProperties.orbitalDistance;
	aiProperties.sinusRotationSpeed = randomFloat(0, 35, 2);
	aiProperties.sinusRotationRadius = randomFloat(0, 100, 2);

	return aiProperties;
}

void AI::update(queue<Entity *> *spawnQueue, Enemy * target, Player * player, UpdateInfo info)
{
	int shots;
	AIProperties properties = target->aiProperties;

	// Fire the weapon as many times as allowed.
	if (target->weapon.isReady(info.elapsedGameTime, info.updateInterval, shots))
	{
		for (int i = 0; i < shots; i++)
		{
			spawnQueue->push(target->weapon.fire(target->getPosition(), Vector2fMath::unitVector(player->getPosition() - target->getPosition()), info.elapsedGameTime, EnemyProjectileEntity));
		}
	}
}