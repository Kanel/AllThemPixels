#include "UserInput.h"

float UserInput::getGamepadEkvivalentValue(Keyboard::Key positive, Keyboard::Key negative)
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

float UserInput::getJoystickPosition(unsigned int joystick, Joystick::Axis axis)
{
	if (Joystick::isConnected(joystick))
	{
		return Joystick::getAxisPosition(joystick, axis);
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

Vector2f UserInput::getJoystickVector(unsigned int joystick, Joystick::Axis x, Joystick::Axis y)
{
	return Vector2f(getJoystickPosition(joystick, x), 
					getJoystickPosition(joystick, y));
}

bool UserInput::isButtonPressed(unsigned int joystick, unsigned int button)
{
	if (Joystick::isConnected(joystick))
	{
		return Joystick::isButtonPressed(joystick, button);
	}
	else
	{
		return false;
	}
}