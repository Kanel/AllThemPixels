#pragma once

#include "Expendable.h"
#include "GameEngine.h"
#include "UpdateInfo.h"
#include <SFML\Graphics.hpp>

using std::vector;
using namespace sf;

class GameEngine;

class GameState : public Expendable, public Drawable
{
protected:
	bool _paused;

public:
	virtual void pause(GameEngine * engine) = 0;
	virtual void resume(GameEngine * engine) = 0;
	virtual bool paused() = 0;

	virtual bool blocking() = 0;

	virtual void handleEvents(GameEngine * engine, vector<Event> events) = 0;
	virtual void update(GameEngine * engine, UpdateInfo info) = 0;
};