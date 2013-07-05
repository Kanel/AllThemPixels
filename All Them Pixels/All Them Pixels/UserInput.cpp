#include "UserInput.h"

int UserInput::getGamepadEkvivalentValue(Keyboard::Key positive, Keyboard::Key negative)
{
	if(Keyboard::isKeyPressed(positive))
	{
		return 100;
	}
	else if (Keyboard::isKeyPressed(negative))
	{
		return -100;
	}
	else
	{
		return 0;
	}
}

float UserInput::getJoystickPosition(Joystick::Axis axis)
{
	if (Joystick::isConnected(0))
	{
		return Joystick::getAxisPosition(0, axis);
	}
	else
	{
		int value = 0;

		switch (axis)
		{
			case sf::Joystick::X:
				value = getGamepadEkvivalentValue(Keyboard::D, Keyboard::A);
				break;

			case sf::Joystick::Y:
				value = getGamepadEkvivalentValue(Keyboard::S, Keyboard::W);
				break;

			case sf::Joystick::Z:
				break;

			case sf::Joystick::R:
				value = getGamepadEkvivalentValue(Keyboard::Down, Keyboard::Up);
				break;

			case sf::Joystick::U:
				value = getGamepadEkvivalentValue(Keyboard::Right, Keyboard::Left);
				break;

			case sf::Joystick::V:
				break;

			case sf::Joystick::PovX:
				break;

			case sf::Joystick::PovY:
				break;

			default:
				break;
		}

		return value;
	}
}

Vector2f UserInput::getJoystickVector(Joystick::Axis x, Joystick::Axis y)
{
	return Vector2f(getJoystickPosition(x), 
					getJoystickPosition(y));
}

bool UserInput::isButtonPressed(unsigned int button)
{
	if (Joystick::isConnected(0))
	{
		return Joystick::isButtonPressed(0, button);
	}
	else
	{
		switch (button)
		{
		case UIC_SCROLL_LEFT:
			return Keyboard::isKeyPressed(Keyboard::E);
		case UIC_SCROLL_RIGHT:
			return Keyboard::isKeyPressed(Keyboard::R);
		case UIC_INCREASE_SKILL:
			return Keyboard::isKeyPressed(Keyboard::T);
		case UIC_DECREASE_SKILL:
			return Keyboard::isKeyPressed(Keyboard::Y);
		}
		return false;
	}
}