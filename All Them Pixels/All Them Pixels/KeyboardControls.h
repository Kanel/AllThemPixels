#pragma once

#include "Controls.h"
#include <cassert>

using namespace sf;

static class KeyboardControls : public Controls
{
protected:
	Keyboard::Key ToggleFullscreenKey;
	Keyboard::Key PauseKey;
	Keyboard::Key DisplayMapKey;
	Keyboard::Key IncreaseSkillKey;
	Keyboard::Key DecreaseSkillKey;
	Keyboard::Key NextSkillKey;
	Keyboard::Key PreviousSkillKey;
	Keyboard::Key MovementHorizontalPositiveKey;
	Keyboard::Key MovementHorizontalNegativeKey;
	Keyboard::Key MovementVerticalPositiveKey;
	Keyboard::Key MovementVerticalNegativeKey;
	Keyboard::Key AimHorizontalPositiveKey;
	Keyboard::Key AimHorizontalNegativeKey;
	Keyboard::Key AimVerticalPositiveKey;
	Keyboard::Key AimVerticalNegativeKey;

public:
	KeyboardControls();

	virtual bool isPressed(Uint32 button);
	virtual Vector2f getVector(Uint32 axisX, Uint32 axisY);

protected:
	virtual float getAxis(Uint32 axis);
	virtual float getAxis(Keyboard::Key positive, Keyboard::Key negative);
};