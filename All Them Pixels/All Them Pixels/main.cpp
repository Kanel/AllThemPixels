#include "GameEngine.h"
#include "PlayingState.h"
#include <SFML/Graphics.hpp>

using namespace sf;

int main(int argc, char ** argv)
{
	Time sleepDuration = milliseconds(GAME_UPDATE_INTERVAL);
	GameEngine engine(WORLD_VIEW_WIDTH, WORLD_VIEW_HEIGHT);

	engine.pushState(new PlayingState());
	
	while (!engine.expended())
    {
		engine.handleEvents();
		engine.update(GAME_UPDATE_INTERVAL);
		engine.draw();
			
		sleep(sleepDuration);
    }
    return 0;
}