#pragma once

enum EntityTypes
{
	UnknownEntity = 1,
	DestructibleEntity = 2,
	EnemyEntity = DestructibleEntity + 4,
	PlayerEntity = DestructibleEntity + 8,
	ProjectileEntity = DestructibleEntity + 16
};