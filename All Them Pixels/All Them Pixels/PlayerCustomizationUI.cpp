#include  "PlayerCustomizationUI.h"

PlayerCustomizationUI::PlayerCustomizationUI(Vector2f position) : wheel(3, 100)
{
	Rect<float> bounds;
	Rect<float> boundsDescription;

	size = Vector2f(400, 200);
	lastSkillChange = 0;
	lastSkillModifed = 0;

	for (int i = 0; i < 4; i++)	wasPressed[i] = false;

	background[0] = Vertex(Vector2f(position.x - (size.x / 2), position.y - (size.y / 2)), UI_BACKGROUND_COLOR);
	background[1] = Vertex(Vector2f(position.x + (size.x / 2), position.y - (size.y / 2)), UI_BACKGROUND_COLOR);
	background[2] = Vertex(Vector2f(position.x + (size.x / 2), position.y + (size.y / 2)), UI_BACKGROUND_COLOR);
	background[3] = Vertex(Vector2f(position.x - (size.x / 2), position.y + (size.y / 2)), UI_BACKGROUND_COLOR);

	font.loadFromFile(UI_FONT_PATH);
	skillTitle.setFont(font);
	skillTitle.setCharacterSize(UI_FONT_SIZE);
	skillTitle.setColor(UI_FONT_COLOR);
	skillDescription.setFont(font);
	skillDescription.setCharacterSize(UI_FONT_SMALL_SIZE);
	skillDescription.setColor(UI_FONT_COLOR);	

	skillTitles[0] = "Cooldown";
	skillTitles[1] = "Damage";
	skillTitles[2] = "Spread";

	skillDescriptions[0] = "The cooldown decreases as it gets alloted more points.";
	skillDescriptions[1] = "The damage increases as it gets alloted more points.";
	skillDescriptions[2] = "The spread increases as it gets alloted more points.";

	skillTitle.setString(skillTitles[wheel.getIndex()]);
	skillDescription.setString(skillDescriptions[wheel.getIndex()]);

	bounds = skillTitle.getLocalBounds();
	boundsDescription = skillDescription.getLocalBounds(); 

	skillTitle.setPosition((-size.x / 2.0f) - bounds.left, (-size.y / 2.0f) - bounds.top);
	skillDescription.setPosition((-size.x / 2.0f) - boundsDescription.left, (-size.y / 2.0f) - bounds.top + bounds.height + UI_TEXT_SPACING);
	wheel.move(100, 0);
}

PlayerConfiguration PlayerCustomizationUI::getConfiguration()
{
	return wheel.getConfiguration();
}
	
PlayerCustomizationUI::Result PlayerCustomizationUI::update(UpdateInfo info)
{
	Result result = Result::NoChange;
	int newIndex;

	if (lastSkillChange + SKILL_CHANGE_COOLDOWN <= info.elapsedGameTime)
	{
		// Check if the wheel should rotate.
		if (UserInput::isButtonPressed(UIC_SCROLL_LEFT))
		{
			wasPressed[0] = true;
		}
		else if (UserInput::isButtonPressed(UIC_SCROLL_RIGHT))
		{
			wasPressed[1] = true;
		}

		// Determine the rotation direction.
		if(wasPressed[0] == true)
		{
			newIndex = (wheel.getIndex() - 1) % wheel.getNumberOfSkills();
			result = Result::Changed;
			wasPressed[0] = false;
		}
		else if(wasPressed[1] == true)
		{
			newIndex = (wheel.getIndex() + 1) % wheel.getNumberOfSkills();
			result = Result::Changed;
			wasPressed[1] = false;
		}

		// Rotate wheel.
		if (result == Result::Changed)
		{
			wheel.setIndex(newIndex);
			skillTitle.setString(skillTitles[wheel.getIndex()]);
			skillDescription.setString(skillDescriptions[wheel.getIndex()]);

			lastSkillChange =info.elapsedGameTime;
		}
	}

	if (lastSkillModifed + SKILL_MODIFY_COOLDOWN <= info.elapsedGameTime)
	{
		float triggervalue = UserInput::getJoystickPosition(Joystick::Axis::Z);

		// Increase ordecrease selected skill.
		if ((triggervalue * triggervalue) > 100)
		{
			wheel.setSkillValue(wheel.getSkillValue() + triggervalue/15);
			result = Result::Changed;
		}
		else if (UserInput::isButtonPressed(UIC_INCREASE_SKILL))
		{
			wheel.setSkillValue(wheel.getSkillValue() + 4);
			result = Result::Changed;
		}
		else if (UserInput::isButtonPressed(UIC_DECREASE_SKILL))
		{
			wheel.setSkillValue(wheel.getSkillValue() - 4);
			result = Result::Changed;			
		}

		if (result == Result::Changed)
		{
			lastSkillModifed = info.elapsedGameTime;
		}
	}
	return result;
}

void PlayerCustomizationUI::align(View view)
{
	setPosition(view.getCenter() + Vector2f(0, (view.getSize().y / 2) - (size.y / 2)));
}

void PlayerCustomizationUI::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(background, 4, PrimitiveType::Quads, states);	
	target.draw(wheel, states);
	target.draw(skillTitle, states);
	target.draw(skillDescription, states);
}