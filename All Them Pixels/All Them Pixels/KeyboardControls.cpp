#include "KeyboardControls.h"

KeyboardControls::KeyboardControls()
{
	ToggleFullscreenKey = Keyboard::F11;
	PauseKey = Keyboard::Escape;
	DisplayMapKey = Keyboard::Space;
	IncreaseSkillKey = Keyboard::Add;
	DecreaseSkillKey = Keyboard::Subtract;
	NextSkillKey = Keyboard::Numpad9;
	PreviousSkillKey = Keyboard::Numpad7;
	MovementHorizontalPositiveKey = Keyboard::D;
	MovementHorizontalNegativeKey = Keyboard::A;
	MovementVerticalPositiveKey = Keyboard::S;
	MovementVerticalNegativeKey = Keyboard::W;
	AimHorizontalPositiveKey = Keyboard::Right;
	AimHorizontalNegativeKey = Keyboard::Left;
	AimVerticalPositiveKey = Keyboard::Down;
	AimVerticalNegativeKey = Keyboard::Up;
}

bool KeyboardControls::isPressed(Uint32 button)
{
	switch (button)
	{
		case Button::ToggleFullscreen:
			return Keyboard::isKeyPressed(ToggleFullscreenKey);

		case Button::Pause:
			return Keyboard::isKeyPressed(PauseKey);

		case Button::DisplayMap:
			return Keyboard::isKeyPressed(DisplayMapKey);

		case Button::IncreaseSkill:
			return Keyboard::isKeyPressed(IncreaseSkillKey);

		case Button::DecreaseSkill:
			return Keyboard::isKeyPressed(DecreaseSkillKey);

		case Button::NextSkill:
			return Keyboard::isKeyPressed(NextSkillKey);

		case Button:: PreviousSkill:
			return Keyboard::isKeyPressed(PreviousSkillKey);

		default:
			assert(false && "Nonexistant button!");
	}
}

Vector2f KeyboardControls::getVector(Uint32 axisX, Uint32 axisY)
{
	return Vector2f(getAxis(axisX), getAxis(axisY));
}

float KeyboardControls::getAxis(Uint32 axis)
{
	switch (axis)
	{
		case Axis::AimHorizontal:
			return getAxis(AimHorizontalPositiveKey, AimHorizontalNegativeKey);

		case Axis::AimVertical:
			return getAxis(AimVerticalPositiveKey, AimVerticalNegativeKey);

		case Axis::MovementHorizontal:
			return getAxis(MovementHorizontalPositiveKey, MovementHorizontalNegativeKey);

		case Axis::MovementVertical:
			return getAxis(MovementVerticalPositiveKey, MovementVerticalNegativeKey);

		default:
			assert(false && "Nonexistant Axis!");
	}
}

float KeyboardControls::getAxis(Keyboard::Key positive, Keyboard::Key negative)
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
