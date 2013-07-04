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
public:
	static AIProperties generate(int difficulty);
	static void update(queue<Entity *> *spawnQueue, Enemy * target, Player * player, UpdateInfo info);
};