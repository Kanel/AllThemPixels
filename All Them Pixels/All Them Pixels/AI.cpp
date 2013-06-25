#include "AI.h"

void AI::update(Territory * territory, Enemy * target, Player * player, UpdateInfo info)
{
	float speed = 2;
	Vector2f direction = Vector2fMath::unitVector(player->getPosition() - target->getPosition());

	target->translate(direction * speed);
}