#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Territory.h"
#include "UpdateInfo.h"

class Territory;
class Player;

static class AI
{
public:
	static void update(Territory * territory, Enemy * target, Player * player, UpdateInfo info);
};