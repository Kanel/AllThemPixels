#pragma once

#include <SFML/Graphics.hpp>
#include <queue>
#include <cassert>

using std::queue;
using namespace sf;

// VertexCollection provides a way of storing and managing vertices. It resizes as it fills 
// up. Intended to be used for batch drawing of vertices of simillar nature.
class VertexCollection : public Drawable
{
private:
	// A queue of slot indices for slots that has been freed up and that can be reused.
	queue<int> freeSlots; 
	int verticesPerShape;
	int nextIndex;
	int size;
	int numberOfVertices;
	int increment;
	Vertex * vertices;
	PrimitiveType type;

private:
	// Allocates a slot and returns its index.
	int allocateSlot();

public:
	// verticesPerShape: The number of vertices in each shape to be stored.
	// size: The number of slots for shapes the collection can store.
	// increment: The number of slots the container will grow by once it's full.
	// type: The primitive type to use when drawing.
	VertexCollection(int verticesPerShape, int size, int increment, PrimitiveType type);
	
	~VertexCollection();

	// Allocates a slot of array space for a shape. 
	int add();

	// Allocates a slot of array space for a shape and fills it with vertices.
	// Vertex vertices[]: Array to use when filling the slot.
	int add(Vertex vertices[]);

	// Deallocates a slot of array space.
	void remove(int index);

	// Will change the number of arrays slots. If its a decrease all slots that can fit will be
	// copied otherwise every slot will be copied.
	void resize(int size);

	Vertex& operator[](const int index);

	virtual void draw(RenderTarget& target, RenderStates states) const;
};