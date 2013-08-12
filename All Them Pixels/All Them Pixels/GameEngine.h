#pragma once

#include "Config.h"
#include "GameState.h"
#include "UpdateInfo.h"
#include <SFML\Graphics.hpp>
#include <vector>

using std::vector;
using namespace sf;

class GameState;

class GameEngine : public Expendable
{
private:
	vector<GameState *> states;
	UpdateInfo info;

public:
	RenderWindow * window;

public:
	GameEngine(int width, int height);
	~GameEngine();

	bool isActive();

	void changeState(GameState * state);
	void pushState(GameState * state);
	void popState();

	void handleEvents();
	void update(int elapsedMilliseconds);
	void draw();
};