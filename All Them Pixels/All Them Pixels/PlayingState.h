#pragma once

#include "World.h"
#include "GameState.h"
#include "PlayerCustomizationUI.h"
#include "PauseState.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class PlayingState : public GameState
{
protected:
	int fadeTimeElapsed;
	Listener listener;
	World world;
	PlayerCustomizationUI playerCustomizationUI;
	bool pausedBeforeLoosOfFocus;

public:
	PlayingState();
	~PlayingState();

	virtual void pause(GameEngine * engine);
	virtual void resume(GameEngine * engine);
	virtual bool paused();

	virtual bool blocking();

	virtual void handleEvents(GameEngine * engine, vector<Event> events);
	virtual void update(GameEngine * engine, UpdateInfo info);
	void draw(RenderTarget& target, RenderStates states) const;
};