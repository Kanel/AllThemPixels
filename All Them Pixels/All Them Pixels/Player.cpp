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
	float x = Joystick::getAxisPosition(0, axisX); 
	float y = -Joystick::getAxisPosition(0, axisY); 
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
	Vector2f vector(Joystick::getAxisPosition(0, x), 
					Joystick::getAxisPosition(0, y));

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

Player::Player(Territory * removePlease, unsigned int hp, Vector2f position) : Destructible(hp, position)
{
	Color color(255, 255, 255);

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

void Player::draw(RenderWindow * window)
{
	window->draw(shape);
	window->draw(aimBox);
}

void Player::update(UpdateInfo info)
{
	Vector2f speed = getJoystickVector(Joystick::Axis::U, Joystick::Axis::R);
	Vector2f spawn = aimBoxPosition;

	int range = sqrt(speed.x * speed.x + speed.y * speed.y) * 4;

	updateAim();
	translate(Vector2f(Joystick::getAxisPosition(0, Joystick::Axis::X) / 200, 
					Joystick::getAxisPosition(0, Joystick::Axis::Y) / 200));
//	translate(getJoystickVector(Joystick::Axis::X, Joystick::Axis::Y));

	if (sqrtf(powf(speed.x, 2) + powf(speed.y, 2)) > 0)
	{
		speed /= sqrtf(powf(speed.x, 2) + powf(speed.y, 2)) / 10;

		for (int i = 0; i < 2; i++)
		{
			removePlease->addEntity(new Projectile(spawn, Vector2f(speed.x - 5 + rand() % 10, speed.y - 5 + rand() % 10), range));
		}
	}
}