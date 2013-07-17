#include "SkillWheel.h"

void SkillWheel::updateCorners(int offset)
{
	float maxValue = 0;
	float modifier;

	for (int i = 0; i < skillValues.size(); i++)
	{
		if (maxValue < *skillValues[i])
		{
			maxValue = *skillValues[i];
		}
	}
	modifier = size / maxValue;	

	if (cornerCount != skillValues.size())
	{
		delete skillCorners;
		
		cornerCount = skillValues.size();
		skillCorners = new Vertex[cornerCount + 2];
	}

	skillCorners[0] = Vector2f();
	
	for (int i = 0, j = 1; i < skillValues.size(); i++, j++)
	{
		skillCorners[j].position.x = cosf(((2 * M_PI) / skillValues.size()) * (i - offset)) * (*skillValues[i]) * modifier;
		skillCorners[j].position.y = sinf(((2 * M_PI) / skillValues.size()) * (i - offset)) * (*skillValues[i]) * modifier;
		skillCorners[j].color = Color::Black;
	}
	skillCorners[cornerCount + 1] = skillCorners[1];
}

SkillWheel::SkillWheel(int skills, float size)
{
	this->size = size;

	cornerCount = 0;
	activeIndex = 0;
	skillCorners = new Vertex[cornerCount + 2];

	updateCorners(0);
}

SkillWheel::~SkillWheel()
{
	delete skillCorners;
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

int SkillWheel::getSkillValue()
{
	return *skillValues[activeIndex];
}

void SkillWheel::setSkillValue(int value)
{	
	*skillValues[activeIndex] = value;

	updateCorners(activeIndex);
}

int SkillWheel::getNumberOfSkills()
{
	return skillValues.size();
}

void SkillWheel::addSkill(int * value)
{
	skillValues.push_back(value);
	updateCorners(activeIndex);
}
	
void SkillWheel::draw(RenderTarget& target, RenderStates states) const
{
	if (cornerCount != 0)
	{
		states.transform *= getTransform();

		target.draw(skillCorners, skillValues.size() + 2, PrimitiveType::TrianglesFan, states);
	}
}