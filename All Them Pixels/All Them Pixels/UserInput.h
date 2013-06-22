#pragma once

#include <SFML\Graphics.hpp>

using namespace sf;

static class UserInput
{
private:
	static float getGamepadEkvivalentValue(Keyboard::Key positive, Keyboard::Key negative);

public:
	static float getJoystickPosition(unsigned int joystick, Joystick::Axis axis);
	static Vector2f getJoystickVector(unsigned int joystick, Joystick::Axis x, Joystick::Axis y);
	static bool isButtonPressed(unsigned int joystick, unsigned int button);
};