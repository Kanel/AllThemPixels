#pragma once

#include "GameState.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class MenuState : public GameState
{
protected:

public:
	MenuState();
	~MenuState();

	virtual void pause();
	virtual void resume();

	virtual bool blocking();

	virtual void handleEvents(GameEngine * engine, vector<Event> events);
	virtual void update(GameEngine * engine, UpdateInfo info);
	void draw(RenderTarget& target, RenderStates states) const;
};