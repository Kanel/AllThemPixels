#include "Player.h"
#include "Territory.h"
#include "UpdateInfo.h"
#include "World.h"
#include "PlayerCustomizationUI.h"
#include "VertexCluster.h"
#include "Sounds.h"
#include "Cero.h";
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace sf;

int main(int argc, char ** argv)
{
	int elapsedGameTime = 0;
	int updateInterval = 10;
	char * windowtitle = "All them Pixels!";
	bool fullscreen = true;
	Uint32 windowStyle = Style::Resize;	
	ContextSettings settings(0, 0, 8, 2, 0);
	VideoMode videoMode(1024, 1024);
    RenderWindow window(videoMode, windowtitle, 7, settings);
	World world(Vector2f(), 2048, 0, 4);
	Territory * t = world.getTerritory(AxialCoordinates(0, 0));
	
	PlayerCustomizationUI ui;
	Time sleepDuration = milliseconds(updateInterval);
	UpdateInfo info;
	Sounds sounds = Sounds();
	//Listener listener;
	//GlareEffect effect(Vector2f());
	Cero cero(Vector2f(), 200);

	info.updateInterval = updateInterval;
	info.elapsedGameTime = 0;

	world.activate(AxialCoordinates());

	while (!world.isCleared() && window.isOpen())
    {
		Event event;
		Player * player = world.getCurrentTerritory()->player;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
			{
				window.close();
			}
		}
		info.elapsedGameTime += updateInterval;

		cero.update(info);
		world.update(info, &sounds);
		window.setView(world.getView(window.getView()));

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

    return 0;
}