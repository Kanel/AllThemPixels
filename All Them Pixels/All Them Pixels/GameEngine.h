#pragma once

#include "Config.h"
#include "GameState.h"
#include "UpdateInfo.h"
#include "Sounds.h"
#include <SFML\Graphics.hpp>
#include <vector>

using std::vector;
using namespace sf;

class GameState;

class GameEngine : public Expendable
{
private:
	bool paused;
	vector<GameState *> states;
	UpdateInfo info;

private:
	void resize(int width, int height);

public:
	RenderWindow * window;
	Sounds sounds;

public:
	GameEngine(int width, int height);
	~GameEngine();

	bool isActive();

	bool isPaused();
	void pause();
	void resume();

	void changeState(GameState * state);
	void pushState(GameState * state);
	void popState();

	void handleEvents();
	void update(int elapsedMilliseconds);
	void draw();
};