#include "Player.h"

void Player::applyTransform(Transform transform, Vertex vertices[], int count)
{
	for (int i = 0; i < count; i++)
	{
		vertices[i].position = transform.transformPoint(vertices[i].position);
	}
}

float Player::getDirection(Joystick::Axis axisX, Joystick::Axis axisY)
{
	float x = UserInput::getJoystickPosition(axisX); 
	float y = -UserInput::getJoystickPosition(axisY); 
	float distance = sqrtf(powf(x, 2) + powf(y, 2));	
	float direction;

	if (x == y && y == 0)
	{
		direction = 0;
	}
	else if (x >= 0 && y >= 0)
	{
		direction = acos(x / distance);
	}
	else if (x <= 0 && y >= 0)
	{
		direction = M_PI - acos((-x) / distance); 
	}
	else if (x >= 0 && y <= 0)
	{
		direction = M_PI + acos((-x) / distance);
	}
	else
	{
		direction = (M_PI * 2) - acos(x / distance);
	}

	return direction * (180 / M_PI);	
}

Vector2f Player::getJoystickVector(Joystick::Axis x, Joystick::Axis y)
{
	Vector2f vector= UserInput::getJoystickVector(x, y);

	vector.x = (powf(vector.x, 2) > 500) ? vector.x / 15 : 0;
	vector.y = (powf(vector.y, 2) > 500) ? vector.y / 15 : 0;

	return vector;
}

void Player::updateAim()
{
	float direction = getDirection(Joystick::Axis::R, Joystick::Axis::U);
	Transform transform;

	transform.rotate(direction - aimDirection, position);
	applyTransform(transform, aimBoxShape, 4);
	
	aimBoxPosition = transform.transformPoint(aimBoxPosition);
	aimDirection = direction;
}

Player::Player(queue<Entity *> *spawnQueue, PlayerConfiguration config, Vector2f position, VertexCollection * vertexSource) : Destructible(config.hp, position), weapon(config.weaponConfig, vertexSource), healthBar(36, 40, 2, 2, hp, hp)
{
	const int size = 20;
	const int aimboxSize = 4;
	Color color(0, 0, 0);

	this->spawnQueue = spawnQueue;
	this->config = config;
	type = PlayerEntity;
	shapeCount = 4;
	isInSafeZone = false;
	aimboxShapeCount = 4;
	playerSkillPoints.common = 0;
	playerSkillPoints.commonUsed = 0;

	shape[0] = Vertex(Vector2f(position.x - size, position.y - size), color);
	shape[1] = Vertex(Vector2f(position.x + size, position.y - size), color);
	shape[2] = Vertex(Vector2f(position.x + size, position.y + size), color);
	shape[3] = Vertex(Vector2f(position.x - size, position.y + size), color);

	aimDirection = 180;
	aimBoxPosition = position;
	aimBoxPosition.y -= 35;
	
	aimBoxShape[0] = Vertex(Vector2f(aimBoxPosition.x - aimboxSize, aimBoxPosition.y - aimboxSize), color);
	aimBoxShape[1] = Vertex(Vector2f(aimBoxPosition.x + aimboxSize, aimBoxPosition.y - aimboxSize), color);
	aimBoxShape[2] = Vertex(Vector2f(aimBoxPosition.x + aimboxSize, aimBoxPosition.y + aimboxSize), color);
	aimBoxShape[3] = Vertex(Vector2f(aimBoxPosition.x - aimboxSize, aimBoxPosition.y + aimboxSize), color);
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

void Player::applyTransform(Transform transform)
{
	Destructible::applyTransform(transform);

	aimBoxPosition = transform.transformPoint(aimBoxPosition);

	applyTransform(transform, shape, shapeCount);
	applyTransform(transform, aimBoxShape, aimboxShapeCount);

	healthBar.setPosition(transform.transformPoint(healthBar.getPosition()));
}

void Player::setHP(int hp)
{
	Destructible::setHP(hp);

	healthBar.setValue(hp);
}

Rect<float> Player::getBoundingBox()
{
	return Collision::getHitBox(shape, shapeCount);
}

ConvexHull Player::getConvexHull()
{
	return Collision::getConvexHull(shape, shapeCount);
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
	float a = 255.0f * ((float)hp / (float)originalHp);

	for (int i = 0; i < 4; i++)
	{
		shape[i].color.a = a;
	}
}

void Player::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(shape, shapeCount, PrimitiveType::Quads);
	target.draw(aimBoxShape, aimboxShapeCount, PrimitiveType::Quads);
	target.draw(healthBar);
}

void Player::update(UpdateInfo info)
{
	int shots;
	Vector2f spawn = aimBoxPosition;
	Vector2f direction = Vector2fMath::unitVector(getJoystickVector(Joystick::Axis::U, Joystick::Axis::R));
	
	speed = getJoystickVector(Joystick::Axis::X, Joystick::Axis::Y) * (float)config.speed;
	aimVector = direction;

	updateAim();
	translate(speed);
	
	if (weapon.isReady(info.elapsedGameTime, info.updateInterval, shots) && !(direction == Vector2f(0.0,0.0)))
	{
		for (int i = 0; i < shots; i++)
		{
			spawnQueue->push(weapon.fire(aimBoxPosition, direction, info.elapsedGameTime, ProjectileEntity));
		}
	}
}