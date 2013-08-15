#pragma once

#include "HexagonGrid.h"
#include "GameState.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class PauseState : public GameState
{
protected:
	vector<Hexagon *> hexagons;
	VertexCollection * collection;

protected:
	void step(AxialCoordinates coordinate, Vector2f origin, HexagonGrid::HexagonDirection direction, int steps, float size, HexagonGrid &grid);
	void generateBackground(Vector2f windowSize);

public:
	PauseState();
	~PauseState();

	virtual void pause(GameEngine * engine);
	virtual void resume(GameEngine * engine);

	virtual bool blocking();

	virtual void handleEvents(GameEngine * engine, vector<Event> events);
	virtual void update(GameEngine * engine, UpdateInfo info);
	void draw(RenderTarget& target, RenderStates states) const;
};