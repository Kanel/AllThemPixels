#include "PlayingState.h"

PlayingState::PlayingState() : world(Vector2f(), WORLD_TERRITORY_RADIUS, WORLD_TERRITORY_SPACING, WORLD_LAYERS)
{
	fadeTimeElapsed = 0;
	paused = false;

	world.activate(AxialCoordinates(0, WORLD_LAYERS));
}

PlayingState::~PlayingState()
{

}

void PlayingState::pause()
{
	paused = true;
}

void PlayingState::resume()
{
	paused = false;
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

					engine->pushState(pauseState);
				}
				else
				{
					pauseState->expend();
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