#include "GamepadControls.h"

GamepadControls::GamepadControls(Uint32 index)
{
	this->index = index;
	ToggleFullscreenKey = Keyboard::F11;
	PauseKey = Keyboard::Pause;
	DisplayMapKey = Keyboard::Space;
	IncreaseSkillKey = Keyboard::Add;
	DecreaseSkillKey = Keyboard::Subtract;
	NextSkillKey = Keyboard::Numpad9;
	PreviousSkillKey = Keyboard::Numpad7;
	MovementHorizontalAxis = Joystick::X;
	MovementVerticalAxis = Joystick::Y;
	AimHorizontalAxis = Joystick::U;
	AimVerticalAxis = Joystick::R;
}

bool GamepadControls::isPressed(Uint32 button)
{
	if (Joystick::isConnected(0))
	{
		switch (button)
		{
			case Button::ToggleFullscreen:
				return Joystick::isButtonPressed(index, ToggleFullscreenKey);

			case Button::Pause:
				return Joystick::isButtonPressed(index, PauseKey);

			case Button::DisplayMap:
				return Joystick::isButtonPressed(index, DisplayMapKey);

			case Button::IncreaseSkill:
				return Joystick::isButtonPressed(index, IncreaseSkillKey);

			case Button::DecreaseSkill:
				return Joystick::isButtonPressed(index, DecreaseSkillKey);

			case Button::NextSkill:
				return Joystick::isButtonPressed(index, NextSkillKey);

			case Button:: PreviousSkill:
				return Joystick::isButtonPressed(index, PreviousSkillKey);

			default:
				assert(false && "Nonexistant button!");
		}
	}
	else
	{
		return false;
	}
}

Vector2f GamepadControls::getVector(Uint32 axisX, Uint32 axisY)
{
	return Vector2f(getAxis(axisX), getAxis(axisY));
}

float GamepadControls::getAxis(Uint32 axis)
{
	if (Joystick::isConnected(index))
	{
		switch (axis)
		{
			case Axis::AimHorizontal:
				return Joystick::getAxisPosition(index, AimHorizontalAxis);

			case Axis::AimVertical:
				return Joystick::getAxisPosition(index, AimVerticalAxis);

			case Axis::MovementHorizontal:
				return Joystick::getAxisPosition(index, MovementHorizontalAxis);

			case Axis::MovementVertical:
				return Joystick::getAxisPosition(index, MovementVerticalAxis);

			default:
				assert(false && "Nonexistant Axis!");
		}
	}
	else
	{
		return 0;
	}
}
