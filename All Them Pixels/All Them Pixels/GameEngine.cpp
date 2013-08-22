#include "GameEngine.h"

void GameEngine::resize(int width, int height)
{
	View view = window->getView();
			
	view.setSize(width, height);
	window->setView(view);
}

GameEngine::GameEngine(int width, int height)
{	
	ContextSettings settings(0, 0, GAME_OPENGL_VERSION_ANTI_ALIASING, GAME_OPENGL_VERSION_MAJOR, GAME_OPENGL_VERSION_MINOR);
	VideoMode videoMode(width, height);
	Image icon;

	windowStyle = Style::Default;   
	window = new RenderWindow(videoMode, GAME_TITLE, windowStyle, settings);
	paused = false;
	info.elapsedGameTime = 0;
	info.updateInterval = 0;

	// Initial controls.
	controls = new KeyboardControls();

	// Load application icon.
	if (icon.loadFromFile("Icon 32x32.png"))
	{
		window->setIcon(32, 32, icon.getPixelsPtr());
	}
	else
	{
		// Errors?
	}
}

GameEngine::~GameEngine()
{
	delete window;
}

bool GameEngine::isActive()
{
	return !expended() && window->isOpen();
}

bool GameEngine::isPaused()
{
	return paused;
}

void GameEngine::pause()
{
	if (!paused)
	{
		paused = true;

		for (int i = 0; i < states.size(); i++)
		{
			states[i]->pause(this);
		}
	}
}

void GameEngine::resume()
{
	if (paused)
	{
		paused = false;

		for (int i = 0; i < states.size(); i++)
		{
			states[i]->resume(this);
		}
	}
}

RenderWindow * GameEngine::getWindow()
{
	return window;
}

Sounds * GameEngine::getSounds()
{
	return &sounds;
}

Controls * GameEngine::getControls()
{
	return controls;
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

void GameEngine::toggleFullscreen()
{
	VideoMode mode;
	ContextSettings settings(0, 0, GAME_OPENGL_VERSION_ANTI_ALIASING, GAME_OPENGL_VERSION_MAJOR, GAME_OPENGL_VERSION_MINOR);

	if (windowStyle == Style::Fullscreen)
	{
		mode = desktop;
		windowStyle = Style::Resize | Style::Close;
	}
	else
	{
		Vector2u view = window->getSize();

		desktop.width = view.x;
		desktop.height = view.y;
		mode = VideoMode::getDesktopMode();
		windowStyle = Style::Fullscreen;
	}

	window->create(mode, GAME_TITLE, windowStyle, settings);
}

void GameEngine::handleEvents()
{
	Event event;
	vector<Event> events;

	while (window->pollEvent(event))
	{
		events.push_back(event);

		switch (event.type)
		{
			case Event::Closed:
				expend();
				break;

			case Event::Resized:
				resize(event.size.width, event.size.height);
				break;

			case Event::JoystickButtonPressed:
				if (event.joystickButton.button == GAMEPAD_A)
				{
					delete controls;
					controls = new GamepadControls(event.joystickButton.joystickId);
				}
				break;

			case Event::JoystickDisconnected:
				delete controls;
				controls = new KeyboardControls();
				break;
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
	if (!paused)
	{		
		info.elapsedGameTime += elapsedMilliseconds;
		info.updateInterval = elapsedMilliseconds;
	}
	else
	{
		info.updateInterval = elapsedMilliseconds;
	}

	int start;

	for (int i = states.size() - 1; 0 <= i; i--)
	{
		if (states[i]->blocking())
		{
			start = i;

			break;
		}
	}

	for (int i = start; i < states.size(); i++)
	{
		states[i]->update(this, info);
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