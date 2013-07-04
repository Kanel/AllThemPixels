#pragma once

#include "Player.h"
#include "UpdateInfo.h"
#include "SkillWheel.h"
#include <SFML\Graphics.hpp>

// Transformable och Drawable verkar snälla men man måste transforma sig massa mer
class PlayerCustomizationUI : public Transformable, public Drawable
{
private:
	Vertex background[4];
	Vector2f size;
	SkillWheel wheel;
	int cooldown;
	int lastChange;

public:
	enum Result
	{
		NoChange,
		Changed
	};

public:
	PlayerCustomizationUI(Vector2f position);

	PlayerConfiguration getConfiguration();
	Result update(UpdateInfo info);

	virtual void align(View view);	
	virtual void draw(RenderTarget& target, RenderStates states) const;
};