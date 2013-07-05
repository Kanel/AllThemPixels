#include "VertexCollection.h"

int VertexCollection::allocateSlot()
{
	int index;

	if (freeSlots.size() > 0)
	{
		index = freeSlots.front();

		freeSlots.pop();
	}
	else
	{
		index = nextIndex;
		nextIndex += verticesPerShape;

		if (index == numberOfVertices)
		{
			resize(size + increment);
		}
	}
	return index;
}

VertexCollection::VertexCollection(int verticesPerShape, int size, int increment, PrimitiveType type)
{
	this->verticesPerShape = verticesPerShape;
	this->type = type;
	this->size = size;
	this->nextIndex = 0;
	this->numberOfVertices = size * verticesPerShape;
	this->increment = increment;
	this->vertices = new Vertex[numberOfVertices];
}

VertexCollection::~VertexCollection()
{
	delete vertices;
}

int VertexCollection::add()
{
	return allocateSlot();
}

int VertexCollection::add(Vertex vertices[])
{
	int index = allocateSlot();

	for(int i = 0; i < verticesPerShape; i++)
	{
		this->vertices[index + i] = vertices[i];
	}
	return index;
}

void VertexCollection::remove(int index)
{
	assert(index >= 0 && index < numberOfVertices);

	for (int i = 0; i < verticesPerShape; i++)
	{
		this->vertices[index + i].position = Vector2f();
	}
	freeSlots.push(index);
}

void VertexCollection::resize(int newSize)
{
	int i;
	int newNumberOfVertices = newSize * verticesPerShape;
	Vertex * newArray = new Vertex[newNumberOfVertices];

	for (i = 0; i < numberOfVertices && i < newNumberOfVertices; i++)
	{
		newArray[i] = vertices[i];
	}
	delete vertices;

	size = newSize;
	nextIndex = i + verticesPerShape;
	numberOfVertices = newNumberOfVertices;
	vertices = newArray;
}

Vertex& VertexCollection::operator[](const int index)
{
	assert(index >= 0 && index < numberOfVertices);

	return vertices[index];
}

void VertexCollection::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(vertices, nextIndex, type, states);
}