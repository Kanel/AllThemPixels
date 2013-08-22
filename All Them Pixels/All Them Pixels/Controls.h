#pragma once

#include "SFML\Graphics.hpp"

#define VECTOR_MAX_STRENGTH sqrtf(powf(100, 2) + powf(100, 2))

using namespace sf;

static class Controls
{
public:
	enum Button
	{
		ToggleFullscreen,
		Pause,
		DisplayMap,
		IncreaseSkill,
		DecreaseSkill,
		NextSkill,
		PreviousSkill
	};

	enum Axis
	{
		MovementHorizontal,
		MovementVertical,
		AimHorizontal,
		AimVertical
	};

public:
	virtual bool isPressed(Uint32 button) = 0;
	virtual Vector2f getVector(Uint32 axisX, Uint32 axisY) = 0;

protected:
	virtual float getAxis(Uint32 axis) = 0;
};