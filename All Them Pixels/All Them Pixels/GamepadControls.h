#pragma once

#include "Controls.h"
#include <cassert>

using namespace sf;

static class GamepadControls : public Controls
{
protected:
	Uint32 index;
	Uint32 ToggleFullscreenKey;
	Uint32 PauseKey;
	Uint32 DisplayMapKey;
	Uint32 IncreaseSkillKey;
	Uint32 DecreaseSkillKey;
	Uint32 NextSkillKey;
	Uint32 PreviousSkillKey;
	Joystick::Axis MovementHorizontalAxis;
	Joystick::Axis MovementVerticalAxis;
	Joystick::Axis AimHorizontalAxis;
	Joystick::Axis AimVerticalAxis;

public:
	GamepadControls(Uint32 index);

	virtual bool isPressed(Uint32 button);
	virtual Vector2f getVector(Uint32 axisX, Uint32 axisY);

protected:
	virtual float getAxis(Uint32 axis);
};