#include  "PlayerCustomizationUI.h"

PlayerCustomizationUI::PlayerCustomizationUI(Vector2f position) : wheel(3, 100)
{
	size = Vector2f(400, 200);
	cooldown = 100;
	lastChange = 0;

	background[0] = Vertex(Vector2f(position.x - (size.x / 2), position.y - (size.y / 2)));
	background[1] = Vertex(Vector2f(position.x + (size.x / 2), position.y - (size.y / 2)));
	background[2] = Vertex(Vector2f(position.x + (size.x / 2), position.y + (size.y / 2)));
	background[3] = Vertex(Vector2f(position.x - (size.x / 2), position.y + (size.y / 2)));

}

PlayerConfiguration PlayerCustomizationUI::getConfiguration()
{
	return wheel.getConfiguration();
}
	
PlayerCustomizationUI::Result PlayerCustomizationUI::update(UpdateInfo info)
{
	Result result = Result::NoChange;

	if (lastChange + cooldown <= info.elapsedGameTime)
	{
		result = Result::Changed;

		if (Keyboard::isKeyPressed(Keyboard::E))
		{
			wheel.setIndex((wheel.getIndex() - 1) % wheel.getNumberOfSkills());

			result = Result::Changed;
		}
		else if (Keyboard::isKeyPressed(Keyboard::R))
		{
			wheel.setIndex((wheel.getIndex() + 1) % wheel.getNumberOfSkills());

			result = Result::Changed;
		}
		else if (Keyboard::isKeyPressed(Keyboard::T))
		{
			wheel.setSkillValue(wheel.getSkillValue() + 4);

			result = Result::Changed;
		}
		
		if (result == Result::Changed)
		{
			lastChange = info.elapsedGameTime;
		}
	}
	return result;
}

void PlayerCustomizationUI::align(View view)
{
	setPosition(view.getCenter() + Vector2f(0, (view.getSize().y / 2) - (size.y / 2)));
	wheel.setPosition(getPosition());
}

void PlayerCustomizationUI::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(background, 4, PrimitiveType::Quads, states);
	target.draw(wheel);
}