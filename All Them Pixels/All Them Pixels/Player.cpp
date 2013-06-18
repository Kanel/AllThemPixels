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

Player::Player(unsigned int hp, Vector2f position) : Destructible(hp, position)
{
	Color color(175, 175, 225);

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
	return Rect<float>();
}

void Player::draw(RenderWindow * window)
{
	window->draw(shape);
	window->draw(aimBox);
}

void Player::update(UpdateInfo info)
{
	updateAim();
	translate(getJoystickVector(Joystick::Axis::X, Joystick::Axis::Y));
}