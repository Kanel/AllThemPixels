#include "PauseState.h"

void PauseState::step(AxialCoordinates coordinate, Vector2f origin, HexagonGrid::HexagonDirection direction, int steps, HexagonGrid &grid)
{
	for (int i = 0; i < steps; i++)
	{
		hexagons.push_back(new Hexagon(origin + grid.getPosition(coordinate), PAUSE_HEXAGON_RADIUS, PAUSE_HEXAGON_COLOR, Hexagon::PointyTopped, collection));

		coordinate = grid.step(coordinate, direction);
	}
}

PauseState::PauseState()
{
	Vector2f origin(PAUSE_HEXAGON_SIZE, PAUSE_HEXAGON_SIZE);
	HexagonGrid grid(Hexagon::PointyTopped, PAUSE_HEXAGON_SIZE);
	AxialCoordinates coordinate;

	collection = new VertexCollection(8, 0, 1, PrimitiveType::TrianglesStrip);

	paused = false;

	// Generate grid.
	step(coordinate, origin, HexagonGrid::DownRight, 3, grid);

	coordinate = grid.step(coordinate, HexagonGrid::Down);

	step(coordinate, origin, HexagonGrid::DownRight, 5, grid);

	coordinate = grid.step(coordinate, HexagonGrid::Down);

	step(coordinate, origin, HexagonGrid::DownRight, 3, grid);

	coordinate = grid.step(coordinate, HexagonGrid::Down);

	step(coordinate, origin, HexagonGrid::DownRight, 3, grid);

	coordinate = grid.step(coordinate, HexagonGrid::Down);
	coordinate = grid.step(coordinate, HexagonGrid::UpLeft);
	coordinate = grid.step(coordinate, HexagonGrid::UpLeft);

	step(coordinate, origin, HexagonGrid::DownRight, 3, grid);
}

PauseState::~PauseState()
{
	for (int i = 0; i < hexagons.size(); i++)
	{
		delete hexagons[i];
	}
	delete collection;
}

void PauseState::pause()
{
	paused = true;
}

void PauseState::resume()
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
	target.draw(text);
	target.draw(*collection, statess);
}