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
		skillValues[i] = 100;
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

	config.hp = 10000;
	config.speed = 1;
	config.weaponConfig.cooldown = (skillValues[0] < 100) ? skillValues[0] - 100 : 1;
	config.weaponConfig.damage = skillValues[1] - 50;
	config.weaponConfig.piercing = 1;
	config.weaponConfig.speed = 5;
	config.weaponConfig.spread = skillValues[2] - 99;
	config.weaponConfig.ttl = 55;

	return config;
}
	
void SkillWheel::draw(RenderTarget& target, RenderStates states) const
{
	states.transform *= getTransform();

	target.draw(skillCorners, skills + 2, PrimitiveType::TrianglesFan, states);
}