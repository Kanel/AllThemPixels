#include "Player.h"
#include "Territory.h"
#include "UpdateInfo.h"
#include "World.h"
#include "PlayerCustomizationUI.h"
#include "VertexCluster.h"
#include "Sounds.h"
#include "config.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

int main(int argc, char ** argv)
{
	int elapsedGameTime = 0;
	int updateInterval = GAME_UPDATE_INTERVAL;
	char * windowtitle = GAME_TITLE;
	bool fullscreen = true;
	Uint32 windowStyle = Style::Resize;	
	ContextSettings settings(0, 0, 8, 2, 0);
	VideoMode videoMode(WORLD_VIEW_WIDTH, WORLD_VIEW_HEIGHT);
    RenderWindow window(videoMode, windowtitle, 7, settings);
	World world(Vector2f(), WORLD_TERRITORY_RADIUS, WORLD_TERRITORY_SPACING, WORLD_LAYERS);
	
	PlayerCustomizationUI ui;
	Time sleepDuration = milliseconds(updateInterval);
	UpdateInfo info;
	Sounds sounds = Sounds();
	Listener listener;
	bool paused = false;

	info.updateInterval = updateInterval;
	info.elapsedGameTime = 0;

	world.activate(AxialCoordinates(0, WORLD_LAYERS));

	while (!world.isCleared() && world.isActive() && window.isOpen())
    {
		Event event;
		Player * player = world.getPlayer();

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
			if (event.type == Event::JoystickButtonReleased)
			{
				if (event.joystickButton.button == GAMEPAD_BACK)
				{
					paused = !paused;
				}
			}
		}
		if (!paused)
		{
			info.elapsedGameTime += updateInterval;

			listener.setPosition(player->getPosition().x, player->getPosition().y, 0);

			world.update(info, &sounds);
			window.setView(world.getView(window.getView()));		
		}
		if (player->getIsInSafeZone())
		{
			ui.align(window.getView());

			if (ui.update(info, player, &sounds) == PlayerCustomizationUI::Changed)
			{
				player->setConfiguration(ui.getConfiguration());
			}
		}

		window.clear();
		window.draw(world);
		
		if (player->getIsInSafeZone())
		{
			window.draw(ui);
		}
		window.display();
			
		// Sleep until the next frame needs to be rendered.
		sleep(sleepDuration);
    }
	if (window.isOpen())
	{
		Color fadeColor = (world.isCleared()) ? Color::White : Color::Black;
		Territory * territory = world.getCurrentTerritory();

		for (int i = 0; i < GAME_FADE_TIME && window.isOpen(); i += sleepDuration.asMilliseconds())
		{
			Event event;

			while (window.pollEvent(event))
			{
				if (event.type == Event::Closed)
				{
					window.close();
				}
			}
			territory->fade(fadeColor, world.getPlayer()->getPosition());

			window.clear();
			window.draw(world);
			window.display();

			sleep(sleepDuration);
		}		
	}

    return 0;
}