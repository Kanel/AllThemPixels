#pragma once

#include "Config.h"
#include "Player.h"
#include <SFML/Graphics.hpp>

using namespace sf;

class SkillWheel : public Transformable, public Drawable
{
private:
	int cornerCount;
	Vertex * skillCorners;
	vector<int *> skillValues;
	int activeIndex;
	float size;

private:
	void updateCorners(int offset);

public:
	SkillWheel(int skills, float size);
	~SkillWheel();

	int getIndex();
	void setIndex(int index);

	int getSkillValue();
	void setSkillValue(int value);

	int getNumberOfSkills();

	void addSkill(int * value);
	
	virtual void draw(RenderTarget& target, RenderStates states) const;
};