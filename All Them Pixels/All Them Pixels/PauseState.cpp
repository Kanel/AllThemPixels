#include "PauseState.h"

void PauseState::step(AxialCoordinates coordinate, Vector2f origin, HexagonGrid::HexagonDirection direction, int steps, float size, HexagonGrid &grid)
{
	for (int i = 0; i < steps; i++)
	{
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

PauseState::PauseState()
{
	paused = false;
	collection = new VertexCollection(8, 0, 1, PrimitiveType::TrianglesStrip);
}

PauseState::~PauseState()
{
	for (int i = 0; i < hexagons.size(); i++)
	{
		delete hexagons[i];
	}
	delete collection;
}

void PauseState::pause(GameEngine * engine)
{
	paused = true;
}

void PauseState::resume(GameEngine * engine)
{
	paused = false;
}

bool PauseState::blocking()
{
	return true;
}

void PauseState::handleEvents(GameEngine * engine, vector<Event> events)
{
	for (int i = 0; i < events.size(); i++)
	{
		if (events[i].type == Event::KeyPressed)
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				expend();
			}
		}
	}
}

void PauseState::update(GameEngine * engine, UpdateInfo info)
{
	if (hexagons.size() != 0)
	{
		for (int i = 0; i < hexagons.size(); i++)
		{
			delete hexagons[i];
		}
		hexagons.clear();
	}	
	generateBackground(engine->window->getView().getSize());
}

void PauseState::draw(RenderTarget& target, RenderStates states) const
{
	Vertex vertices[4];
	Vector2f center = target.getView().getCenter();
	Vector2f size = target.getView().getSize();
	Vector2f upperLeftCorner = center - size * 0.5f;
	Rect<float> box;
	Font font;
	Text text;

	font.loadFromFile(PAUSE_FONT_PATH);
	text.setFont(font);
	text.setCharacterSize(PAUSE_FONT_SIZE);
	text.setColor(PAUSE_FONT_COLOR);
	text.setString(PAUSE_TEXT);

	box = text.getLocalBounds();

	text.setPosition(center - Vector2f(box.left + box.width * 0.5f, box.top + box.height * 0.5f));

	vertices[0] = Vertex(Vector2f(center.x - size.x / 2, center.y + size.y / 2), PAUSE_BACKGROUND);
	vertices[1] = Vertex(Vector2f(center.x + size.x / 2, center.y + size.y / 2), PAUSE_BACKGROUND);
	vertices[2] = Vertex(Vector2f(center.x + size.x / 2, center.y - size.y / 2), PAUSE_BACKGROUND);
	vertices[3] = Vertex(Vector2f(center.x - size.x / 2, center.y - size.y / 2), PAUSE_BACKGROUND);

	RenderStates statess;

	statess.transform.translate(upperLeftCorner);

	target.draw(vertices, 4, PrimitiveType::Quads);
	target.draw(*collection, statess);
	target.draw(text);
}