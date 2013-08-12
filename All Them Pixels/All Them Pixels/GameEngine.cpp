#include "GameEngine.h"

GameEngine::GameEngine(int width, int height)
{
	//Uint32 windowStyle = Style::Resize;	
	ContextSettings settings(0, 0, 8, 2, 0);
	VideoMode videoMode(width, height);
   
	window = new RenderWindow(videoMode, GAME_TITLE, 7, settings);
	info.elapsedGameTime = 0;
	info.updateInterval = 0;
}

GameEngine::~GameEngine()
{
	delete window;
}

bool GameEngine::isActive()
{
	return !expended() && window->isOpen();
}

void GameEngine::changeState(GameState * state)
{
	for (int i = 0; i < states.size(); i++)
	{
		delete states[i];
	}
	states.clear();

	pushState(state);
}

void GameEngine::pushState(GameState * state)
{
	states.push_back(state);
}

void GameEngine::popState()
{
	delete states[states.size() - 1];

	states.resize(states.size() - 1);
}

void GameEngine::handleEvents()
{
	Event event;
	vector<Event> events;

	while (window->pollEvent(event))
	{
		events.push_back(event);

		if (event.type == Event::Closed)
		{
			expend();
		}
	}
	for (int i = 0; i < states.size(); i++)
	{
		states[i]->handleEvents(this, events);
	}
}

void GameEngine::update(int elapsedMilliseconds)
{
	vector<GameState *> activeStates;

	// Clean up expended states.
	for (int i = 0; i < states.size(); i++)
	{
		if (!states[i]->expended())
		{
			activeStates.push_back(states[i]);
		}
		else
		{
			delete states[i];
		}
	}

	states = activeStates;

	// Update all active states.
	info.elapsedGameTime += elapsedMilliseconds;
	info.updateInterval = elapsedMilliseconds;

	for (int i = 0; i < states.size(); i++)
	{
		states[i]->update(this, info);

		if (states[i]->blocking())
		{
			break;
		}
	}
}

void GameEngine::draw()
{
	window->clear();

	for (int i = 0; i < states.size(); i++)
	{
		window->draw(*states[i]);
	}
	window->display();
}