#include  "PlayerCustomizationUI.h"

void PlayerCustomizationUI::prepareText(Text &text, Font &font, int characterSize)
{
	text.setFont(font);
	text.setCharacterSize(characterSize);
	text.setColor(UI_FONT_COLOR);
}

void PlayerCustomizationUI::addCustomization(String title, String description, int * value)
{
	Custimazation customization;

	customization.title = title;
	customization.description = description;
	customization.value = value;

	wheel.addSkill(value);
	custimazations.push_back(customization);
}

void PlayerCustomizationUI::updateSkillInfo()
{
	skillTitle.setString(custimazations[wheel.getIndex()].title);
	skillDescription.setString(custimazations[wheel.getIndex()].description);
}

PlayerCustomizationUI::PlayerCustomizationUI(Vector2f position) : wheel(3, 100)
{
	Rect<float> bounds;
	Rect<float> boundsDescription;
	Rect<float> boundsSkillPoints;

	// Load default values for skills.
	config.hp = PLAYER_BASE_HP;
	config.speed = PLAYER_BASE_SPEED;
	config.weaponConfig.cooldown = PLAYER_BASE_WEAPON_COOLDOWN;
	config.weaponConfig.damage = PLAYER_BASE_WEAPON_COOLDOWN;
	config.weaponConfig.piercing = PLAYER_BASE_WEAPON_PIERCING;
	config.weaponConfig.speed = PLAYER_BASE_WEAPON_SPEED;
	config.weaponConfig.spread = PLAYER_BASE_WEAPON_SPREAD;
	config.weaponConfig.ttl = PLAYER_BASE_WEAPON_TTL;

	// UI dimensions
	size = Vector2f(UI_PLAYER_CUSTOMIZATION_WIDTH, UI_PLAYER_CUSTOMIZATION_HEIGHT);
	
	// Initlize 
	lastSkillChange = 0;
	lastSkillModifed = 0;
	increasePressed = false;
	decreasePressed = false;

	// Create background.
	background[0] = Vertex(Vector2f(position.x - (size.x / 2), position.y - (size.y / 2)), UI_BACKGROUND_COLOR);
	background[1] = Vertex(Vector2f(position.x + (size.x / 2), position.y - (size.y / 2)), UI_BACKGROUND_COLOR);
	background[2] = Vertex(Vector2f(position.x + (size.x / 2), position.y + (size.y / 2)), UI_BACKGROUND_COLOR);
	background[3] = Vertex(Vector2f(position.x - (size.x / 2), position.y + (size.y / 2)), UI_BACKGROUND_COLOR);

	// Load font.
	font.loadFromFile(UI_FONT_PATH);

	// Add all skills that can be modified.
	addCustomization("Cooldown",	"The cooldown decreases as it gets alloted more points.",		&config.weaponConfig.cooldown);
	addCustomization("Damage",		"The damage increases as it gets alloted more points.",			&config.weaponConfig.damage);
	addCustomization("Piercing",	"The piercing increases as it gets alloted more points.",		&config.weaponConfig.piercing);
	addCustomization("Speed",		"The speed increases as it gets alloted more points.",			&config.weaponConfig.speed);
	addCustomization("Spread",		"The spread increases as it gets alloted more points.",			&config.weaponConfig.spread);
	addCustomization("TTL",			"The time to live increases as it gets alloted more points.",   &config.weaponConfig.ttl);

	// Prepare labels.
	prepareText(skillTitle, font, UI_FONT_SIZE);
	prepareText(skillDescription, font, UI_FONT_SMALL_SIZE);
	prepareText(skillPoints, font, UI_FONT_SMALL_SIZE);

	// Select default skill.
	updateSkillInfo();
	skillPoints.setString("Common Points:");

	// Get bounding boxes for labels.
	bounds = skillTitle.getLocalBounds();
	boundsDescription = skillDescription.getLocalBounds();
	boundsSkillPoints = skillPoints.getLocalBounds();

	// Position labels.
	skillTitle.setPosition((-size.x / 2.0f) - bounds.left, (-size.y / 2.0f) - bounds.top);
	skillDescription.setPosition((-size.x / 2.0f) - boundsDescription.left, (-size.y / 2.0f) - bounds.top + bounds.height + UI_TEXT_SPACING);
	skillPoints.setPosition((-size.x / 2.0f) - boundsSkillPoints.left, (size.y / 2.0f) - boundsSkillPoints.height - boundsSkillPoints.top);
	wheel.move(100, 0);
}

PlayerConfiguration PlayerCustomizationUI::getConfiguration()
{
	PlayerConfiguration config = this->config;

	config.weaponConfig.cooldown = (200 - config.weaponConfig.cooldown  > 0) ? 200 - config.weaponConfig.cooldown: 1;

	return config;
}
	
PlayerCustomizationUI::Result PlayerCustomizationUI::update(UpdateInfo info, Player * player, Sounds * sounds)
{
	Result result = NoChange;	

	int newIndex;

	// Determine if the skill wheel should be rotated to the left and if so the what 
	// skill.
	if (UserInput::isButtonPressed(UIC_SCROLL_LEFT))
	{
		increasePressed = true;
	}
	else if(increasePressed == true) //OnButtonUp
	{
		sounds->play(SoundTypes::SkillScroll);
		newIndex = (wheel.getIndex() - 1) % wheel.getNumberOfSkills();
		newIndex = (newIndex < 0) ? wheel.getNumberOfSkills() + newIndex : newIndex;
		result = Changed;
		increasePressed = false;
	}

	// Determine if the skill wheel should be rotated to the right and if so the what 
	// skill.
	if (UserInput::isButtonPressed(UIC_SCROLL_RIGHT))
	{
		decreasePressed = true;
	}
	else if(decreasePressed == true) //OnButtonUp
	{
		sounds->play(SoundTypes::SkillScroll);
		newIndex = (wheel.getIndex() + 1) % wheel.getNumberOfSkills();
		result = Changed;
		decreasePressed = false;
	}

	// Rotate skill wheel.
	if (result == Changed)
	{
		wheel.setIndex(newIndex);
		updateSkillInfo();

		lastSkillChange = info.elapsedGameTime;
	}

	// Determine if a skills value should be modified and if so update the skill value.
	if (lastSkillModifed + SKILL_MODIFY_COOLDOWN <= info.elapsedGameTime)
	{
		char buffer[100];
		int value = 0;
		float triggervalue = UserInput::getJoystickPosition(Joystick::Axis::Z);
		PlayerSkillPoints * playerSkillPoints = player->getPlayerSkillPoints();

		// Determine if selected skill should be increased or decreased and
		// get the amount of change.
		if (powf(triggervalue, 2) > 100)
		{
			value = triggervalue / 15;
			result = Changed;
		}
		else if (UserInput::isButtonPressed(UIC_INCREASE_SKILL))
		{
			value = UI_PLAYER_CUSTOMIZATION_MODIFY_VALUE;
			result = Changed;
		}
		else if (UserInput::isButtonPressed(UIC_DECREASE_SKILL))
		{
			value = -UI_PLAYER_CUSTOMIZATION_MODIFY_VALUE;	
			result = Changed;
		}

		// Apply change if possible.
		if (result == Changed)
		{
			if (value > 0)
			{
				if (playerSkillPoints->common - playerSkillPoints->commonUsed >= value)
				{
					playerSkillPoints->commonUsed += value;
					wheel.setSkillValue(wheel.getSkillValue() + value);		
				}
			}
			else
			{
				if (playerSkillPoints->commonUsed >= abs(value))
				{
					playerSkillPoints->commonUsed += value;
					wheel.setSkillValue(wheel.getSkillValue() + value);		
				}
				else if (playerSkillPoints->commonUsed >= 1)
				{
					playerSkillPoints->commonUsed = 0;
					wheel.setSkillValue(wheel.getSkillValue() - playerSkillPoints->commonUsed);
				}
			}
		}

		// Update display of unused points.
		snprintf(buffer, 100, "Common Points: %d", playerSkillPoints->common - playerSkillPoints->commonUsed);
		
		skillPoints.setString(buffer);

		// Save the moment in time it was modified.
		if (result == Changed)
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
	target.draw(skillPoints, states);
}