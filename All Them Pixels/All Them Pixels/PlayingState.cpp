#include "PlayingState.h"

PlayingState::PlayingState() : world(Vector2f(), WORLD_TERRITORY_RADIUS, WORLD_TERRITORY_SPACING, WORLD_LAYERS)
{
	fadeTimeElapsed = 0;
	paused = false;
	pauseStatesUsed = 0;

	world.activate(AxialCoordinates(0, WORLD_LAYERS));
}

PlayingState::~PlayingState()
{
	if (pauseStatesUsed != 0)
	{
		pauseState->expend();
	}
}

void PlayingState::pause(GameEngine * engine)
{
	paused = true;
	pauseState = new PauseState();
	pauseStatesUsed++;

	engine->pushState(pauseState);
}

void PlayingState::resume(GameEngine * engine)
{
	paused = false;
	pauseStatesUsed--;

	if (pauseStatesUsed == 0)
	{
		pauseState->expend();
	}
}

bool PlayingState::blocking()
{
	return true;
}

void PlayingState::handleEvents(GameEngine * engine, vector<Event> events)
{
	for (int i = 0; i < events.size(); i++)
	{
		if (events[i].type == Event::JoystickButtonReleased)
		{
			if (events[i].joystickButton.button == GAMEPAD_START)
			{
				paused = !paused;

				if (paused)
				{
					pauseState = new PauseState();
					pauseStatesUsed++;

					engine->pushState(pauseState);
				}
				else
				{
					pauseStatesUsed--;

					if (pauseStatesUsed == 0)
					{
						pauseState->expend();
					}
				}
			}
		}
	}
}

void PlayingState::update(GameEngine * engine, UpdateInfo info)
{
	if (!paused)
	{
		if (!world.isCleared() && world.isActive())
		{
			world.update(info, &sounds);
			playerCustomizationUI.update(info, world.getPlayer(), &sounds);

			engine->window->setView(world.getView(engine->window->getView()));
			playerCustomizationUI.align(engine->window->getView());
		}
		else if (fadeTimeElapsed <= GAME_FADE_TIME)
		{
			Color fadeColor = (world.isCleared()) ? Color::White : Color::Black;
			Territory * territory = world.getCurrentTerritory();

			territory->fade(fadeColor, world.getPlayer()->getPosition());

			fadeTimeElapsed  += info.updateInterval;
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