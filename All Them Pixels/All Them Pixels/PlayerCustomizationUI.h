#pragma once

#include "Player.h"
#include "UpdateInfo.h"
#include "SkillWheel.h"
#include "UserInput.h"
#include "Sounds.h"
#include <stdio.h>
#include <SFML/Graphics.hpp>

#if _MSC_VER
#define snprintf _snprintf
#endif

// Transformable och Drawable verkar snälla men man måste transforma sig massa mer
class PlayerCustomizationUI : public Transformable, public Drawable
{
private:
	struct Custimazation
	{
		String title;
		String description;
		int * value;
	};

private:
	vector<Custimazation> custimazations;
	Vertex background[4];
	Vector2f size;
	SkillWheel wheel;
	Font font;
	Text skillTitle;
	Text skillDescription;
	Text skillPoints;
	PlayerConfiguration config;
	int lastSkillChange;
	int lastSkillModifed;
	bool wasPressed[4];
	Sounds sounds;

private:
	void prepareText(Text &text, Font &font, int characterSize);
	void addCustomization(String title, String description, int * value);
	void updateSkillInfo();

public:
	enum Result
	{
		NoChange,
		Changed
	};

public:
	PlayerCustomizationUI(Vector2f position);

	PlayerConfiguration getConfiguration();
	Result update(UpdateInfo info, Player * player, Sounds * sounds);

	virtual void align(View view);	
	virtual void draw(RenderTarget& target, RenderStates states) const;
};