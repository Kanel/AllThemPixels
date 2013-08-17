#pragma once

#include "HexagonGrid.h"
#include "GameState.h"
#include "Position.h"
#include "ValueBar.h"
#include <SFML\Graphics.hpp>

using namespace sf;

class PauseState : public GameState
{
protected:
	Vector2f position;
	vector<Hexagon *> hexagons;
	VertexCollection * collection;
	Font font;
	Text text[6];
	vector<AxialCoordinates> pattern;
	AxialCoordinates valueBarsPattern[3][4];
	ValueBar * valueBars[3];
	GameState * target;
	bool pauseButtonReleased;

protected:
	void step(AxialCoordinates coordinate, Vector2f origin, HexagonGrid::HexagonDirection direction, int steps, float size, HexagonGrid &grid);
	void generateBackground(Vector2f windowSize);
	void prepareText(Text &text, Font &font, int fontSize, Color color, String string);
	void positionText(Vector2f position, HorizontalPosition horizontalPosition, VerticalPosition verticalPosition, Text &text);
	bool containsPoint(AxialCoordinates * coordinates, int count, AxialCoordinates point);

public:
	PauseState(GameState * target);
	~PauseState();

	virtual void pause(GameEngine * engine);
	virtual void resume(GameEngine * engine);
	virtual bool paused();

	virtual bool blocking();

	virtual void handleEvents(GameEngine * engine, vector<Event> events);
	virtual void update(GameEngine * engine, UpdateInfo info);
	void draw(RenderTarget& target, RenderStates states) const;
};