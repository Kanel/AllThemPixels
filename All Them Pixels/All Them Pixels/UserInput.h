#pragma once

#define UIC_SCROLL_LEFT 4 //left bumper
#define UIC_SCROLL_RIGHT 5 //right bumper
#define UIC_INCREASE_SKILL 6 //start
#define UIC_DECREASE_SKILL 7 //back

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