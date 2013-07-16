#include "SkillWheel.h"

void SkillWheel::updateCorners(int offset)
{
	float maxValue = 0;
	float modifier;

	for (int i = 0; i < skills; i++)
	{
		if (maxValue < skillValues[i])
		{
			maxValue = skillValues[i];
		}
	}
	modifier = size / maxValue;	

	for (int i = 0; i < skills; i++)
	{
		skillCorners[i + 1].position.x = cosf(((2 * M_PI) / 4) * (i - offset)) * skillValues[i] * modifier;
		skillCorners[i + 1].position.y = sinf(((2 * M_PI) / 4) * (i - offset)) * skillValues[i] * modifier;
	}
	skillCorners[skills + 1] = skillCorners[1];
}

SkillWheel::SkillWheel(int skills, float size)
{
	Color color[] = {
		Color(255, 0, 0),
		Color(0, 255, 0),
		Color(0, 0, 255)
	};

	this->skills = skills;
	this->size = size;

	activeIndex = 0;
	skillCorners = new Vertex[skills + 2];
	skillValues = new float[skills];

	skillCorners[0].position = Vector2f();

	for (int i = 0; i < skills; i++)
	{
		skillValues[i] = 1;
	}
	for (int i = 0; i < skills; i++)
	{
		skillCorners[i + 1].color = color[i % 3];
	}
	updateCorners(0);
}

SkillWheel::~SkillWheel()
{
	delete skillCorners;
	delete skillValues;
}

int SkillWheel::getIndex()
{
	return activeIndex;
}

void SkillWheel::setIndex(int index)
{
	activeIndex = index;
	
	updateCorners(activeIndex);
}

float SkillWheel::getSkillValue()
{
	return skillValues[activeIndex];
}

void SkillWheel::setSkillValue(float value)
{	
	skillValues[activeIndex] = value;

	updateCorners(activeIndex);
}

int SkillWheel::getNumberOfSkills()
{
	return skills;
}

PlayerConfiguration SkillWheel::getConfiguration()
{
	PlayerConfiguration config;

	config.hp = PLAYER_BASE_HP;
	config.speed = PLAYER_BASE_SPEED;
	config.weaponConfig.cooldown = (PLAYER_BASE_WEAPON_COOLDOWN - skillValues[0] > 0) ? PLAYER_BASE_WEAPON_COOLDOWN - skillValues[0] : 1;
	config.weaponConfig.damage = PLAYER_BASE_WEAPON_COOLDOWN;
	config.weaponConfig.piercing = PLAYER_BASE_WEAPON_PIERCING;
	config.weaponConfig.speed = PLAYER_BASE_WEAPON_SPEED + skillValues[1];
	config.weaponConfig.spread = PLAYER_BASE_WEAPON_SPREAD + skillValues[2];
	config.weaponConfig.ttl = PLAYER_BASE_WEAPON_TTL;

	return config;
}
	
void SkillWheel::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(skillCorners, skills + 2, PrimitiveType::TrianglesFan, states);
}