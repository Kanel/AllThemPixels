#include "Player.h"

void Player::applyTransform(Transform transform, Vertex vertices[], int count)
{
	for (int i = 0; i < count; i++)
	{
		vertices[i].position = transform.transformPoint(vertices[i].position);
	}

	boundingBox = Collision::getHitBox(playerShape, playerVertexCount);
}

void Player::applyColor(Color color, Vertex vertices[], int count)
{
	for (int i = 0; i < count; i++)
	{
		vertices[i].color = color;
	}
}

float Player::getStrength(Controls * controls)
{
	Vector2f vector = controls->getVector(Controls::MovementHorizontal, Controls::MovementVertical);

	return Vector2fMath::length(vector) / VECTOR_MAX_STRENGTH;
}

void Player::updateRotation(Controls * controls)
{
	Vector2f aimVector = controls->getVector(Controls::AimHorizontal, Controls::AimVertical);
	float direction = Vector2fMath::getAngle(aimVector);
	Transform transform;

	if (Vector2fMath::length(aimVector) >= GAMEPAD_JOYSTICK_THRESHOLD)
	{	
		transform.rotate(direction - aimDirection, position);
		applyTransform(transform, playerShape, playerVertexCount);
	
		aimBoxPosition = transform.transformPoint(aimBoxPosition);
		aimDirection = direction;
	}
}

Player::Player(queue<Entity *> *spawnQueue, PlayerConfiguration config, Vector2f position) : Destructible(config.hp, position), projectileVertexSource(8, 50, 50, PrimitiveType::TrianglesStrip), weapon(config.weaponConfig, &projectileVertexSource), healthBar(36, 40, 2, 2, hp, hp)
{
	const float size = PLAYER_SIZE;
	const int aimboxSize = PLAYER_AIM_BOX_SIZE;
	Color color = PLAYER_COLOR;
	Transform transform;

	this->spawnQueue = spawnQueue;
	this->config = config;
	type = PlayerEntity;
	shapeCount = 4;
	isInSafeZone = false;
	aimboxShapeCount = 4;
	playerSkillPoints.common = 0;
	playerSkillPoints.commonUsed = 0;

	leftIndex = 12;
	rightIndex = 26;
	bottomIndex = 40;
	coreIndex = 0;
	playerVertexCount = 57;
	leftVertexCount = 14;
	rightVertexCount = 14;
	bottomVertexCount = 15;
	coreVertexCount = 12;

	aimDirection = 270;
	aimBoxPosition = position;
	aimBoxPosition.y -= 35;
	
	playerShapeCore = &playerShape[coreIndex];
	playerShapeBottom = &playerShape[bottomIndex];
	playerShapeLeft = &playerShape[leftIndex];
	playerShapeRight = &playerShape[rightIndex];

	// Core
	playerShapeCore[0] = Vertex(Vector2f(0.0000, 0.0000) * size, color);
	playerShapeCore[1] = Vertex(Vector2f(0.0000, 0.1501) * size, color);
	playerShapeCore[2] = Vertex(Vector2f(0.0884, 0.1214) * size, color);
	playerShapeCore[3] = Vertex(Vector2f(0.1431, 0.0461) * size, color);
	playerShapeCore[4] = Vertex(Vector2f(0.1431, -0.0468) * size, color);
	playerShapeCore[5] = Vertex(Vector2f(0.0884, -0.1221) * size, color);
	playerShapeCore[6] = Vertex(Vector2f(0.0000, -0.1508) * size, color);
	playerShapeCore[7] = Vertex(Vector2f(-0.0884, -0.1221) * size, color);
	playerShapeCore[8] = Vertex(Vector2f(-0.1431, -0.0468) * size, color);
	playerShapeCore[9] = Vertex(Vector2f(-0.1431, 0.0461) * size, color);
	playerShapeCore[10] = Vertex(Vector2f(-0.0884, 0.1214) * size, color);
	playerShapeCore[11] = Vertex(Vector2f(0.0000, 0.1501) * size, color);

	// Bottom
	playerShapeBottom[0] = Vertex(Vector2f(0.0000, -0.7010) * size, color);
	playerShapeBottom[1] = Vertex(Vector2f(-0.1441, -1.0003) * size, color);
	playerShapeBottom[2] = Vertex(Vector2f(-0.1936, -0.6996) * size, color);
	playerShapeBottom[3] = Vertex(Vector2f(-0.3848, -0.4571) * size, color);
	playerShapeBottom[4] = Vertex(Vector2f(-0.3848, -0.2916) * size, color);
	playerShapeBottom[5] = Vertex(Vector2f(-0.1560, -0.1074) * size, color);
	playerShapeBottom[6] = Vertex(Vector2f(-0.1180, -0.1626) * size, color);
	playerShapeBottom[7] = Vertex(Vector2f(0.0000, -0.2009) * size, color);
	playerShapeBottom[8] = Vertex(Vector2f(0.1180, -0.1626) * size, color);
	playerShapeBottom[9] = Vertex(Vector2f(0.1560, -0.1133) * size, color);
	playerShapeBottom[10] = Vertex(Vector2f(0.3934, -0.2876) * size, color);
	playerShapeBottom[11] = Vertex(Vector2f(0.3934, -0.4571) * size, color);
	playerShapeBottom[12] = Vertex(Vector2f(0.2021, -0.6996) * size, color);
	playerShapeBottom[13] = Vertex(Vector2f(0.1527, -1.0003) * size, color);
	playerShapeBottom[14] = Vertex(Vector2f(-0.1441, -1.0003) * size, color);

	// Left
	playerShapeLeft[0] = Vertex(Vector2f(-0.3920, 0.3970) * size, color);
	playerShapeLeft[1] = Vertex(Vector2f(-0.4470, -0.2303) * size, color);
	playerShapeLeft[2] = Vertex(Vector2f(-0.5506, 0.1474) * size, color);
	playerShapeLeft[3] = Vertex(Vector2f(-0.7740, 0.3585) * size, color);
	playerShapeLeft[4] = Vertex(Vector2f(-0.6695, 0.7016) * size, color);
	playerShapeLeft[5] = Vertex(Vector2f(-0.3848, 0.9421) * size, color);
	playerShapeLeft[6] = Vertex(Vector2f(-0.1564, 0.9421) * size, color);
	playerShapeLeft[7] = Vertex(Vector2f(-0.0968, 0.6963) * size, color);
	playerShapeLeft[8] = Vertex(Vector2f(0.0000, 0.6963) * size, color);
	playerShapeLeft[9] = Vertex(Vector2f(0.0000, 0.2002) * size, color);
	playerShapeLeft[10] = Vertex(Vector2f(-0.1059, 0.1619) * size, color);
	playerShapeLeft[11] = Vertex(Vector2f(-0.1788, 0.0616) * size, color);
	playerShapeLeft[12] = Vertex(Vector2f(-0.1788, -0.0188) * size, color);
	playerShapeLeft[13] = Vertex(Vector2f(-0.4470, -0.2303) * size, color);

	// Right
	playerShapeRight[0] = Vertex(Vector2f(0.3920, 0.3970) * size, color);
	playerShapeRight[1] = Vertex(Vector2f(0.4470, -0.2303) * size, color);
	playerShapeRight[2] = Vertex(Vector2f(0.5506, 0.1474) * size, color);
	playerShapeRight[3] = Vertex(Vector2f(0.7740, 0.3585) * size, color);
	playerShapeRight[4] = Vertex(Vector2f(0.6695, 0.7016) * size, color);
	playerShapeRight[5] = Vertex(Vector2f(0.3848, 0.9421) * size, color);
	playerShapeRight[6] = Vertex(Vector2f(0.1564, 0.9421) * size, color);
	playerShapeRight[7] = Vertex(Vector2f(0.0968, 0.6963) * size, color);
	playerShapeRight[8] = Vertex(Vector2f(0.0000, 0.6963) * size, color);
	playerShapeRight[9] = Vertex(Vector2f(0.0000, 0.2002) * size, color);
	playerShapeRight[10] = Vertex(Vector2f(0.1059, 0.1619) * size, color);
	playerShapeRight[11] = Vertex(Vector2f(0.1788, 0.0616) * size, color);
	playerShapeRight[12] = Vertex(Vector2f(0.1788, -0.0188) * size, color);
	playerShapeRight[13] = Vertex(Vector2f(0.4470, -0.2303) * size, color);

	// Align
	transform.rotate(180, position);
	applyTransform(transform, playerShape, playerVertexCount);
}

bool Player::getIsInSafeZone()
{
	return isInSafeZone;
}

void Player::setIsInSafeZone(bool isInSafeZone)
{
	this->isInSafeZone = isInSafeZone;
}

int Player::getSafeZoneIndex()
{
	return safeZoneIndex;
}

void Player::setSafeZoneIndex(int safeZoneIndex)
{
	this->safeZoneIndex = safeZoneIndex;
}

Vector2f Player::getSpeed()
{
	return speed;
}

void Player::applyTransform(Transform &transform)
{
	Destructible::applyTransform(transform);

	aimBoxPosition = transform.transformPoint(aimBoxPosition);

	applyTransform(transform, playerShape, playerVertexCount);
	//applyTransform(transform, aimBoxShape, aimboxShapeCount);

	healthBar.setPosition(transform.transformPoint(healthBar.getPosition()));
}

void Player::setHP(int hp)
{
	Destructible::setHP(hp);

	healthBar.setValue(hp);
}

Rect<float> Player::getBoundingBox()
{
	return boundingBox;
}

ConvexHull Player::getConvexHull()
{
	return Collision::getConvexHull(playerShape, playerVertexCount);
}

void Player::setConfiguration(PlayerConfiguration config)
{
	this->config = config;

	weapon.setConfiguration(config.weaponConfig);
}

void Player::addSkillPoints(SkillPoints skillPoints)
{
	if (skillPoints.type == SkillPointType::Common)
	{
		playerSkillPoints.common+= skillPoints.amount;
	}
}

PlayerSkillPoints * Player::getPlayerSkillPoints()
{
	return &playerSkillPoints;
}

void Player::fade()
{
	float a = (float)UCHAR_MAX * (hp / (float)originalHp);
	Color color = playerShape[0].color;

	color.a = a;

	applyColor(color, playerShape, playerVertexCount);
}

void Player::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(playerShapeCore, coreVertexCount, PrimitiveType::TrianglesFan);
	target.draw(playerShapeBottom, bottomVertexCount, PrimitiveType::TrianglesFan);
	target.draw(playerShapeLeft, leftVertexCount, PrimitiveType::TrianglesFan);
	target.draw(playerShapeRight, rightVertexCount, PrimitiveType::TrianglesFan);
	target.draw(healthBar);
	target.draw(projectileVertexSource);
}

void Player::update(UpdateInfo info, Controls * controls)
{
	int shots;
	float strength = getStrength(controls);
	Vector2f spawn = aimBoxPosition;
	Vector2f aimJoystick = controls->getVector(Controls::AimHorizontal, Controls::AimVertical);
	
	speed = Vector2fMath::unitVector(controls->getVector(Controls::MovementHorizontal, Controls::MovementVertical)) * (config.speed * strength * (info.updateInterval / 1000.0f));
	this->aim = Vector2fMath::unitVector(aimJoystick);

	if (Vector2fMath::length(aim) != 0)
	{
		Vector2f vec = controls->getVector(Controls::MovementHorizontal, Controls::MovementVertical);
		bool spath = true;
	}

	updateRotation(controls);
	translate(speed);
	
	if (weapon.isReady(info.elapsedGameTime, info.updateInterval, shots) && Vector2fMath::length(aimJoystick) > GAMEPAD_JOYSTICK_THRESHOLD)
	{
		for (int i = 0; i < shots; i++)
		{
			spawnQueue->push(weapon.fire(aimBoxPosition, aim, info.elapsedGameTime, ProjectileEntity));
		}
	}
}