#pragma once

#include "Player.h"
#include <SFML\Graphics.hpp>

// Transformable och Drawable verkar snälla men man måste transforma sig massa mer
class PlayerCustomizationUI : public Transformable, public Drawable
{
private:
	PlayerConfiguration config;
	Vertex background[4];
	Vector2f size;

public:
	enum Result
	{
		NoChange,
		Changed
	};

public:
	PlayerCustomizationUI(Vector2f position);

	PlayerConfiguration getConfiguration();
	Result update(Vector2f mousePosition);

	virtual void align(View view);	
	virtual void draw(RenderTarget& target, RenderStates states) const;
};