#include  "PlayerCustomizationUI.h"

PlayerCustomizationUI::PlayerCustomizationUI(Vector2f position)
{
	size = Vector2f(400, 200);

	background[0] = Vertex(Vector2f(position.x - (size.x / 2), position.y - (size.y / 2)));
	background[1] = Vertex(Vector2f(position.x + (size.x / 2), position.y - (size.y / 2)));
	background[2] = Vertex(Vector2f(position.x + (size.x / 2), position.y + (size.y / 2)));
	background[3] = Vertex(Vector2f(position.x - (size.x / 2), position.y + (size.y / 2)));

	config.hp = 10000;
	config.speed = 10;
	config.weaponConfig.cooldown = 100;
	config.weaponConfig.damage = 100;
	config.weaponConfig.piercing = 1;
	config.weaponConfig.speed = 10;
	config.weaponConfig.spread = 5;
	config.weaponConfig.ttl = 55;
}

PlayerConfiguration PlayerCustomizationUI::getConfiguration()
{
	return config;
}
	
PlayerCustomizationUI::Result PlayerCustomizationUI::update(Vector2f mousePosition)
{
	Rect<float> boundingBox = getTransform().transformRect(Collision::getHitBox(background, 4));

	if (Collision::containsPoint(boundingBox, mousePosition))
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			config.weaponConfig.cooldown = 0;
			config.weaponConfig.spread = 110;
		}
		else if (Mouse::isButtonPressed(Mouse::Middle))
		{
			
		}
		else if (Mouse::isButtonPressed(Mouse::Right))
		{
			config.weaponConfig.cooldown = 100;
			config.weaponConfig.spread = 5;
		}
		return Result::Changed;
	}
	else
	{
		return Result::NoChange;
	}
}

void PlayerCustomizationUI::align(View view)
{
	setPosition(view.getCenter() + Vector2f(0, (view.getSize().y / 2) - (size.y / 2)));
}

void PlayerCustomizationUI::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(background, 4, PrimitiveType::Quads, states);
}