#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Territory.h"
#include "UpdateInfo.h"
#include "Weapon.h"

class Territory;
class Player;

struct AIProperties
{
	float speed;
	int aimavoidance;
	float playerspace;
	bool righthanded;
};

static class AI
{
public:
	static AIProperties generate(int difficulty);
	static void update(Territory * territory, Enemy * target, Player * player, Weapon * weapon, AIProperties * properties, UpdateInfo info);
};