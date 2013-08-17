#include "PauseState.h"

void PauseState::step(AxialCoordinates coordinate, Vector2f origin, HexagonGrid::HexagonDirection direction, int steps, float size, HexagonGrid &grid)
{
	for (int i = 0; i < steps; i++)
	{
		pattern.push_back(coordinate);
		hexagons.push_back(new Hexagon(origin + grid.getPosition(coordinate), size, PAUSE_HEXAGON_COLOR, Hexagon::PointyTopped, collection));

		coordinate = grid.step(coordinate, direction);
	}
}

void PauseState::generateBackground(Vector2f windowSize)
{
	float hexagonSize = (PAUSE_HEXAGON_RADIUS_RATIO * windowSize.y > 80) ? 80 : PAUSE_HEXAGON_RADIUS_RATIO * windowSize.y;
	Vector2f origin(hexagonSize, hexagonSize);
	HexagonGrid grid(Hexagon::PointyTopped, hexagonSize + PAUSE_HEXAGON_PADDING);
	AxialCoordinates coordinate;	

	// Generate grid.
	step(coordinate, origin, HexagonGrid::DownRight, 3, hexagonSize, grid);

	coordinate = grid.step(coordinate, HexagonGrid::Down);

	step(coordinate, origin, HexagonGrid::DownRight, 5, hexagonSize, grid);

	coordinate = grid.step(coordinate, HexagonGrid::Down);

	step(coordinate, origin, HexagonGrid::DownRight, 3, hexagonSize, grid);

	coordinate = grid.step(coordinate, HexagonGrid::Down);

	step(coordinate, origin, HexagonGrid::DownRight, 3, hexagonSize, grid);

	coordinate = grid.step(coordinate, HexagonGrid::Down);
	coordinate = grid.step(coordinate, HexagonGrid::UpLeft);
	coordinate = grid.step(coordinate, HexagonGrid::UpLeft);

	step(coordinate, origin, HexagonGrid::DownRight, 3, hexagonSize, grid);
}

void PauseState::prepareText(Text &text, Font &font, int fontSize, Color color, String string)
{
	text.setFont(font);
	text.setCharacterSize(fontSize);
	text.setColor(color);
	text.setString(string);
}

void PauseState::positionText(Vector2f position, HorizontalPosition horizontalPosition, VerticalPosition verticalPosition, Text &text)
{
	Rect<float> dimensions = text.getLocalBounds();

	switch (horizontalPosition)
	{
		case HorizontalPosition::Left:
			position.x -= dimensions.left + dimensions.width;
			break;

		case HorizontalPosition::Middle:
			position.x -= dimensions.left + dimensions.width * 0.5f;
			break;

		case HorizontalPosition::Right:
			position.x += -dimensions.left + dimensions.width;
			break;
	}

	switch (verticalPosition)
	{
		case VerticalPosition::Top:
			position.y -= dimensions.top +  dimensions.height;
			break;

		case VerticalPosition::Center:
			position.y -= dimensions.top + dimensions.height * 0.5f;
			break;

		case VerticalPosition::Bottom:
			position.y += -dimensions.top + dimensions.height;
			break;
	}
	text.setPosition(position);
}

bool PauseState::containsPoint(AxialCoordinates * coordinates, int count, AxialCoordinates point)
{
	for (int i = 0; i < count; i++)
	{
		if (coordinates[i] == point)
		{
			return true;
		}
	}
	return false;
}

PauseState::PauseState(GameState * target)
{
	this->target = target;

	pauseButtonReleased = false;
	_paused = false;
	collection = new VertexCollection(8, 0, 1, PrimitiveType::TrianglesStrip);

	valueBars[0] = new ValueBar(Vector2f(), Vector2f(), 4, 20, 0, 100, SOUNDS_VOLUME_MUSIC);
	valueBars[1] = new ValueBar(Vector2f(), Vector2f(), 4, 20, 0, 100, SOUNDS_VOLUME_EFFECTS);
	valueBars[2] = new ValueBar(Vector2f(), Vector2f(), 4, 20, 0, 100, SOUNDS_VOLUME_UI);
	
	valueBarsPattern[0][0] = AxialCoordinates(0, 0);
	valueBarsPattern[0][1] = AxialCoordinates(0, 1);
	valueBarsPattern[0][2] = AxialCoordinates(0, 2);
	valueBarsPattern[0][3] = AxialCoordinates(0, 3);

	valueBarsPattern[1][0] = AxialCoordinates(1, 0);
	valueBarsPattern[1][1] = AxialCoordinates(1, 1);
	valueBarsPattern[1][2] = AxialCoordinates(1, 2);
	valueBarsPattern[1][3] = AxialCoordinates(1, 3);

	valueBarsPattern[2][0] = AxialCoordinates(2, 0);
	valueBarsPattern[2][1] = AxialCoordinates(2, 1);
	valueBarsPattern[2][2] = AxialCoordinates(2, 2);
	valueBarsPattern[2][3] = AxialCoordinates(2, 3);
	
	font.loadFromFile(PAUSE_FONT_PATH);

	prepareText(text[0], font, PAUSE_FONT_SIZE, PAUSE_FONT_COLOR, PAUSE_TEXT);
	prepareText(text[1], font, PAUSE_FONT_SIZE_SMALL, PAUSE_FONT_COLOR, PAUSE_MUSIC_TEXT);
	prepareText(text[2], font, PAUSE_FONT_SIZE_SMALL, PAUSE_FONT_COLOR, PAUSE_EFFECTS_TEXT);
	prepareText(text[3], font, PAUSE_FONT_SIZE_SMALL, PAUSE_FONT_COLOR, PAUSE_UI_TEXT);
	prepareText(text[4], font, PAUSE_FONT_SIZE_SMALL, PAUSE_FONT_COLOR, PAUSE_RESUME_TEXT);
	prepareText(text[5], font, PAUSE_FONT_SIZE_SMALL, PAUSE_FONT_COLOR, PAUSE_QUIT_TEXT);
}

PauseState::~PauseState()
{
	for (int i = 0; i < hexagons.size(); i++)
	{
		delete hexagons[i];
	}
	delete collection;
	delete valueBars[0];
	delete valueBars[1];
}

void PauseState::pause(GameEngine * engine)
{
	_paused = true;
}

void PauseState::resume(GameEngine * engine)
{
	_paused = false;
}

bool PauseState::paused()
{
	return _paused;
}

bool PauseState::blocking()
{
	return true;
}

void PauseState::handleEvents(GameEngine * engine, vector<Event> events)
{
	for (int i = 0; i < events.size(); i++)
	{
		switch (events[i].type)
		{
			case Event::JoystickButtonPressed:
				if (events[i].joystickButton.button == GAMEPAD_START && pauseButtonReleased)
				{
					target->resume(engine);

					expend();
				}
				break;

			case Event::JoystickButtonReleased:
				if (events[i].joystickButton.button == GAMEPAD_START)
				{
					pauseButtonReleased = true;
				}
				break;

			case Event::MouseButtonReleased:
				Vector2f size = engine->window->getView().getSize();
				float hexagonSize = (PAUSE_HEXAGON_RADIUS_RATIO * size.y > 80) ? 80 : PAUSE_HEXAGON_RADIUS_RATIO * size.y;
				Vector2f origin(hexagonSize + PAUSE_HEXAGON_PADDING, hexagonSize + PAUSE_HEXAGON_PADDING);
				Vector2i mousePosition = Mouse::getPosition(*engine->window);
				HexagonGrid grid(Hexagon::PointyTopped, hexagonSize + PAUSE_HEXAGON_PADDING);
				AxialCoordinates mouseCoordinate = grid.getAxialCoordinates(Vector2f(mousePosition.x, mousePosition.y) - origin);

				if (pattern[7] == mouseCoordinate)
				{
					target->resume(engine);

					expend();
				}
				break;
		}
			
	}
}

void PauseState::update(GameEngine * engine, UpdateInfo info)
{
	if (!paused())
	{
		Vector2f center = engine->window->getView().getCenter();
		Vector2f size = engine->window->getView().getSize();
		float hexagonSize = (PAUSE_HEXAGON_RADIUS_RATIO * size.y > 80) ? 80 : PAUSE_HEXAGON_RADIUS_RATIO * size.y;
		Vector2f origin(hexagonSize + PAUSE_HEXAGON_PADDING, hexagonSize + PAUSE_HEXAGON_PADDING);
		Vector2i mousePosition = Mouse::getPosition(*engine->window);
		HexagonGrid grid(Hexagon::PointyTopped, hexagonSize + PAUSE_HEXAGON_PADDING);
		Transform movement;
		AxialCoordinates mouseCoordinate = grid.getAxialCoordinates(Vector2f(mousePosition.x, mousePosition.y) - origin);

		movement.translate(position - center);

		// Delete old hexagons
		if (hexagons.size() != 0)
		{
			for (int i = 0; i < hexagons.size(); i++)
			{
				delete hexagons[i];
			}
			hexagons.clear();
			pattern.clear();
		}

		// Create new hexagons
		generateBackground(engine->window->getView().getSize());

		// Update positions
		positionText(center, HorizontalPosition::Middle, VerticalPosition::Center, text[0]);
		positionText(origin + grid.getPosition(0, 0), HorizontalPosition::Middle, VerticalPosition::Center, text[1]);
		positionText(origin + grid.getPosition(1, 0), HorizontalPosition::Middle, VerticalPosition::Center, text[2]);
		positionText(origin + grid.getPosition(2, 0), HorizontalPosition::Middle, VerticalPosition::Center, text[3]);
		positionText(origin + grid.getPosition(4, 1), HorizontalPosition::Middle, VerticalPosition::Center, text[4]);
		positionText(origin + grid.getPosition(0, 4), HorizontalPosition::Middle, VerticalPosition::Center, text[5]);

		text[0].setColor(PAUSE_FONT_COLOR);
		text[1].setColor(PAUSE_FONT_COLOR);
		text[2].setColor(PAUSE_FONT_COLOR);
		text[3].setColor(PAUSE_FONT_COLOR);
		text[4].setColor(PAUSE_FONT_COLOR);
		text[5].setColor(PAUSE_FONT_COLOR);
	
		// Hover
		if (containsPoint(valueBarsPattern[0], 4, mouseCoordinate))
		{
			hexagons[0]->setColor(Color(255, 255, 255, 200));
			hexagons[3]->setColor(Color(255, 255, 255, 200));
			hexagons[8]->setColor(Color(255, 255, 255, 200));
			hexagons[11]->setColor(Color(255, 255, 255, 200));
			text[1].setColor(PAUSE_FONT_HOVE_COLOR);
		}
		if (containsPoint(valueBarsPattern[1], 4, mouseCoordinate))
		{
			hexagons[1]->setColor(Color(255, 255, 255, 200));
			hexagons[4]->setColor(Color(255, 255, 255, 200));
			hexagons[9]->setColor(Color(255, 255, 255, 200));
			hexagons[12]->setColor(Color(255, 255, 255, 200));
			text[2].setColor(PAUSE_FONT_HOVE_COLOR);
		}
		if (containsPoint(valueBarsPattern[2], 4, mouseCoordinate))
		{
			hexagons[2]->setColor(Color(255, 255, 255, 200));
			hexagons[5]->setColor(Color(255, 255, 255, 200));
			hexagons[10]->setColor(Color(255, 255, 255, 200));
			hexagons[13]->setColor(Color(255, 255, 255, 200));
			text[3].setColor(PAUSE_FONT_HOVE_COLOR);
		}
		if (pattern[7] == mouseCoordinate)
		{
			hexagons[7]->setColor(Color(255, 255, 255, 200));
			text[4].setColor(PAUSE_FONT_HOVE_COLOR);
		}
		if (pattern[16] == mouseCoordinate)
		{
			hexagons[16]->setColor(Color(255, 255, 255, 200));
			text[5].setColor(PAUSE_FONT_HOVE_COLOR);
		}

		// Update bars
		valueBars[0]->setPoints(origin + grid.getPosition(0, 1), origin + grid.getPosition(0, 3));
		valueBars[0]->update(Vector2f(mousePosition.x, mousePosition.y));
		valueBars[1]->setPoints(origin + grid.getPosition(1, 1), origin + grid.getPosition(1, 3));
		valueBars[1]->update(Vector2f(mousePosition.x, mousePosition.y));
		valueBars[2]->setPoints(origin + grid.getPosition(2, 1), origin + grid.getPosition(2, 3));
		valueBars[2]->update(Vector2f(mousePosition.x, mousePosition.y));

		engine->sounds.setMusicVolume(valueBars[0]->getValue());
		engine->sounds.setEffectsVolume(valueBars[1]->getValue());
		engine->sounds.setUIVolume(valueBars[2]->getValue());

		// Save current view position.
		position = center;
	}

	// Dispose if target has resumed.
	if (!target->paused())
	{
		expend();
	}
}

void PauseState::draw(RenderTarget& target, RenderStates states) const
{
	Vertex vertices[4];
	Vertex box[4];
	Vector2f center = target.getView().getCenter();
	Vector2f size = target.getView().getSize();
	Vector2f upperLeftCorner = center - size * 0.5f;
	Rect<float> boundingBox = text[0].getLocalBounds();
	Vector2f rectangle(boundingBox.width + 24, boundingBox.height + 24);

	vertices[0] = Vertex(Vector2f(center.x - size.x / 2, center.y + size.y / 2), PAUSE_BACKGROUND);
	vertices[1] = Vertex(Vector2f(center.x + size.x / 2, center.y + size.y / 2), PAUSE_BACKGROUND);
	vertices[2] = Vertex(Vector2f(center.x + size.x / 2, center.y - size.y / 2), PAUSE_BACKGROUND);
	vertices[3] = Vertex(Vector2f(center.x - size.x / 2, center.y - size.y / 2), PAUSE_BACKGROUND);

	box[0] = Vertex(Vector2f(center.x - rectangle.x / 2, center.y + rectangle.y / 2), PAUSE_HEXAGON_COLOR);
	box[1] = Vertex(Vector2f(center.x + rectangle.x / 2, center.y + rectangle.y / 2), PAUSE_HEXAGON_COLOR);
	box[2] = Vertex(Vector2f(center.x + rectangle.x / 2, center.y - rectangle.y / 2), PAUSE_HEXAGON_COLOR);
	box[3] = Vertex(Vector2f(center.x - rectangle.x / 2, center.y - rectangle.y / 2), PAUSE_HEXAGON_COLOR);
	
	RenderStates statess;

	statess.transform.translate(upperLeftCorner);

	target.draw(vertices, 4, PrimitiveType::Quads);
	target.draw(box, 4, PrimitiveType::Quads);
	target.draw(*collection, statess);
	target.draw(text[0]);
	target.draw(text[1], statess);
	target.draw(text[2], statess);
	target.draw(text[3], statess);
	target.draw(text[4], statess);
	target.draw(text[5], statess);
	target.draw(*valueBars[0], statess);
	target.draw(*valueBars[1], statess);
	target.draw(*valueBars[2], statess);
}