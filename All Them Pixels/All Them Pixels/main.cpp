#include "GameEngine.h"
#include "PlayingState.h"
#include <SFML/Graphics.hpp>

using namespace sf;

int main(int argc, char ** argv)
{
	Uint64 lastUpdateTime = 0;
	Clock time;
	GameEngine engine(WORLD_VIEW_WIDTH, WORLD_VIEW_HEIGHT);

	engine.pushState(new PlayingState());
	
	while (!engine.expended())
    {
		Uint64 currentTime = time.getElapsedTime().asMilliseconds();
		Uint64 elapsedTime = currentTime - lastUpdateTime;
		
		elapsedTime = (elapsedTime > GAME_MAXIMUM_UPDATE_INTERVAL) ? GAME_MAXIMUM_UPDATE_INTERVAL : elapsedTime;

		engine.handleEvents();
		engine.update(elapsedTime);
		engine.draw();

		lastUpdateTime = currentTime;
    }
    return 0;
}