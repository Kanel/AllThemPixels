#include "Player.h"

void Player::applyTransform(Transform transform, VertexArray &vertecies)
{
	for (int i = 0; i < vertecies.getVertexCount(); i++)
	{
		vertecies[i].position = transform.transformPoint(vertecies[i].position);
	}
}

float Player::getDirection(Joystick::Axis axisX, Joystick::Axis axisY)
{
	float x = UserInput::getJoystickPosition(0, axisX); 
	float y = -UserInput::getJoystickPosition(0, axisY); 
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
	Vector2f vector= UserInput::getJoystickVector(0, x, y);

	vector.x = (powf(vector.x, 2) > 400) ? vector.x / 15 : 0;
	vector.y = (powf(vector.y, 2) > 400) ? vector.y / 15 : 0;

	return vector;
}

void Player::updateAim()
{
	float direction = getDirection(Joystick::Axis::R, Joystick::Axis::U);
	Transform transform;

	transform.rotate(direction - aimDirection, position);
	applyTransform(transform, aimBox);
	
	aimBoxPosition = transform.transformPoint(aimBoxPosition);
	aimDirection = direction;
}

Player::Player(Territory * removePlease, unsigned int hp, Vector2f position) : Destructible(hp, position), weapon(0, 10, 1000, 1, 10, 100)
{
	Color color(0, 0, 0);

	this->removePlease = removePlease;
	type = EntityTypes::PlayerEntity;

	shape.setPrimitiveType(PrimitiveType::Quads);
	shape.append(Vertex(Vector2f(position.x - 20, position.y - 20), color));
	shape.append(Vertex(Vector2f(position.x + 20, position.y - 20), color));
	shape.append(Vertex(Vector2f(position.x + 20, position.y + 20), color));
	shape.append(Vertex(Vector2f(position.x - 20, position.y + 20), color));

	aimDirection = 180;
	aimBoxPosition = position;
	aimBoxPosition.y -= 35;
		
	aimBox.setPrimitiveType(PrimitiveType::Quads);
	aimBox.append(Vertex(Vector2f(aimBoxPosition.x - 4, aimBoxPosition.y - 4), color));
	aimBox.append(Vertex(Vector2f(aimBoxPosition.x + 4, aimBoxPosition.y - 4), color));
	aimBox.append(Vertex(Vector2f(aimBoxPosition.x + 4, aimBoxPosition.y + 4), color));
	aimBox.append(Vertex(Vector2f(aimBoxPosition.x - 4, aimBoxPosition.y + 4), color));
	
	this->hp = 1000;
}

void Player::applyTransform(Transform transform)
{
	Destructible::applyTransform(transform);

	aimBoxPosition = transform.transformPoint(aimBoxPosition);

	applyTransform(transform, shape);
	applyTransform(transform, aimBox);
}

Rect<float> Player::getBoundingBox()
{
	// Todo: stop cheating
	return Rect<float>(position.x - 20, position.y - 20, 40, 40);
}

ConvexHull Player::getConvexHull()
{
	vector<Vector2f> vertecies;

	for (int i = 0; i < shape.getVertexCount(); i++)
	{
		vertecies.push_back(shape[i].position);
	}

	return MonotoneChain::getConvexHull(vertecies);
}

void Player::fade()
{
	for (int i = 0; i < shape.getVertexCount(); i++)
	{
		shape[i].color.a = 255 * ((float)hp / (float)originalHp);
	}
}

void Player::draw(RenderWindow * window)
{
	window->draw(shape);
	window->draw(aimBox);
}

void Player::update(UpdateInfo info)
{
	Vector2f spawn = aimBoxPosition;
	Vector2f direction = Vector2fMath::unitVector(UserInput::getJoystickVector(0, Joystick::Axis::U, Joystick::Axis::R));
	this->aimVector = direction;
	updateAim();
	translate(UserInput::getJoystickVector(0, Joystick::Axis::X, Joystick::Axis::Y) / 20.0f);
	
	if (weapon.isReady(info.elapsedGameTime) && !_isnan(direction.x) && !_isnan(direction.y))
	{
		removePlease->addEntity(weapon.fire(aimBoxPosition, direction, info.elapsedGameTime));
	}
}