#pragma once

#include "Player.h"
#include "Enemy.h"
#include "UpdateInfo.h"
#include "Weapon.h"
#include <queue>

using std::queue;

class Player;

static class AI
{
private:
	static Vector2f aproximation(Vector2f vector);

public:
	static AIProperties generate(int difficulty);
	static void update(queue<Entity *> *spawnQueue, Enemy * target, Player * player, UpdateInfo info);
};