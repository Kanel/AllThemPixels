#pragma once

#include "Config.h"

#define UIC_SCROLL_LEFT GAMEPAD_LB
#define UIC_SCROLL_RIGHT GAMEPAD_RB
#define UIC_INCREASE_SKILL GAMEPAD_START
#define UIC_DECREASE_SKILL GAMEPAD_BACK

#include <SFML/Graphics.hpp>

using namespace sf;

static class UserInput
{
private:
	static int getGamepadEkvivalentValue(Keyboard::Key positive, Keyboard::Key negative);

public:
	static float getJoystickPosition(Joystick::Axis axis);
	static Vector2f getJoystickVector(Joystick::Axis x, Joystick::Axis y);
	static bool isButtonPressed(unsigned int button);
};