#include  "PlayerCustomizationUI.h"

PlayerCustomizationUI::PlayerCustomizationUI(Vector2f position) : wheel(3, 100)
{
	size = Vector2f(400, 200);
	cooldown = 20;
	lastChange = 0;

	for (int i = 0; i < 4; i++)	wasPressed[i] = false;

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

	if (UserInput::isButtonPressed(UIC_SCROLL_LEFT))
	{
		wasPressed[0] = true;
	}
	else if(wasPressed[0] == true)
	{
		wheel.setIndex((wheel.getIndex() - 1) % wheel.getNumberOfSkills());
		result = Result::Changed;
		wasPressed[0] = false;
	}
	if (UserInput::isButtonPressed(UIC_SCROLL_RIGHT))
	{
		wasPressed[1] = true;
	}
	else if(wasPressed[1] == true)
	{
		wheel.setIndex((wheel.getIndex() + 1) % wheel.getNumberOfSkills());
		result = Result::Changed;
		wasPressed[1] = false;
	}
	if (lastChange + cooldown <= info.elapsedGameTime)
	{
		float triggervalue = UserInput::getJoystickPosition(Joystick::Axis::Z);
		if ((triggervalue * triggervalue) > 100)
		{
			wheel.setSkillValue(wheel.getSkillValue() + triggervalue/15);
			result = Result::Changed;
			lastChange = info.elapsedGameTime;
		}
		if (UserInput::isButtonPressed(UIC_INCREASE_SKILL))
		{
			wheel.setSkillValue(wheel.getSkillValue() + 4);
			result = Result::Changed;
			lastChange = info.elapsedGameTime;
		}
		if (UserInput::isButtonPressed(UIC_DECREASE_SKILL))
		{
			wheel.setSkillValue(wheel.getSkillValue() - 4);
			result = Result::Changed;
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