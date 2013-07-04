#pragma once

#include "Player.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class SkillWheel : public Transformable, public Drawable
{
private:
	Vertex * skillCorners;
	float * skillValues;
	int activeIndex;
	int skills;
	float size;

private:
	void updateCorners(int offset);

public:
	SkillWheel(int skills, float size);
	~SkillWheel();

	int getIndex();
	void setIndex(int index);

	float getSkillValue();
	void setSkillValue(float value);

	int getNumberOfSkills();

	PlayerConfiguration getConfiguration();
	
	virtual void draw(RenderTarget& target, RenderStates states) const;
};