#include "PlayingState.h"

PlayingState::PlayingState() : world(Vector2f(), WORLD_TERRITORY_RADIUS, WORLD_TERRITORY_SPACING, WORLD_LAYERS)
{
	fadeTimeElapsed = 0;
	_paused = false;

	world.activate(AxialCoordinates(0, WORLD_LAYERS));
}

PlayingState::~PlayingState()
{
}

void PlayingState::pause(GameEngine * engine)
{
	if (!paused())
	{
		_paused = true;		

		engine->pushState(new PauseState(this));
	}
}

void PlayingState::resume(GameEngine * engine)
{
	_paused = false;
}

bool PlayingState::paused()
{
	return _paused;
}

bool PlayingState::blocking()
{
	return true;
}

void PlayingState::handleEvents(GameEngine * engine, vector<Event> events)
{
	for (int i = 0; i < events.size(); i++)
	{			
		switch (events[i].type)
		{
			case Event::JoystickButtonPressed:
				switch (events[i].joystickButton.button)
				{
					case GAMEPAD_START:
						pause(engine);
						break;

					case GAMEPAD_Y:
						engine->toggleFullscreen();
						break;
				}
				break;

			case Event::LostFocus:
				pausedBeforeLoosOfFocus = _paused;
				pause(engine);
				break;

			case Event::GainedFocus:
				if (!pausedBeforeLoosOfFocus)
				{
					resume(engine);
				}				
				break;
		}
	}
}

void PlayingState::update(GameEngine * engine, UpdateInfo info)
{
	if (!_paused)
	{
		if (!world.isCleared() && world.isActive())
		{
			Vector2f playerPosition = world.getPlayer()->getPosition();
			RenderWindow * window = engine->getWindow();
			Sounds * sounds = engine->getSounds();

			world.update(info, engine->getControls(), sounds);

			if (playerCustomizationUI.update(info, world.getPlayer(), engine->getControls(), sounds) == PlayerCustomizationUI::Changed)
			{
				world.getPlayer()->setConfiguration(playerCustomizationUI.getConfiguration());
			}

			window->setView(world.getView(window->getView()));
			playerCustomizationUI.align(window->getView());
			listener.setPosition(playerPosition.x, playerPosition.y, 0);
		}
		else if (fadeTimeElapsed <= GAME_FADE_TIME)
		{
			Color fadeColor = (world.isCleared()) ? Color::White : Color::Black;
			Territory * territory = world.getCurrentTerritory();

			territory->fade(fadeColor, world.getPlayer()->getPosition());

			fadeTimeElapsed += info.updateInterval;
		}
		else
		{
			expend();
		}
	}
}

void PlayingState::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(world);

	if (world.getPlayer()->getIsInSafeZone())
	{
		target.draw(playerCustomizationUI);
	}
}