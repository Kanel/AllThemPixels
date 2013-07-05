#pragma once

#include <SFML\Graphics.hpp>
#include <queue>
#include <cassert>

using std::queue;
using namespace sf;

class VertexCollection : public Drawable
{
private:
	queue<int> freeSlots;
	int verticesPerShape;
	int nextIndex;
	int size;
	int numberOfVertices;
	Vertex * vertices;
	PrimitiveType type;

public:
	VertexCollection();
	VertexCollection(int verticesPerShape, int size, PrimitiveType type);
	~VertexCollection();

	int add();
	int add(Vertex vertices[]);
	void remove(int index);

	void resize(int size);

	Vertex& operator[](const int index);

	virtual void draw(RenderTarget& target, RenderStates states) const;
};