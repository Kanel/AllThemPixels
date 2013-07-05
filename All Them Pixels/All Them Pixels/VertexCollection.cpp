#include "VertexCollection.h"

VertexCollection::VertexCollection()
{
	this->verticesPerShape = 0;
	this->type = PrimitiveType::TrianglesStrip;
	this->size = 0;
	this->nextIndex = 0;
	this->numberOfVertices = 0;
	this->vertices = NULL;
}

VertexCollection::VertexCollection(int verticesPerShape, int size, PrimitiveType type)
{
	this->verticesPerShape = verticesPerShape;
	this->type = type;
	this->size = size;
	this->nextIndex = 0;
	this->numberOfVertices = size * verticesPerShape;
	this->vertices = new Vertex[numberOfVertices];
}

VertexCollection::~VertexCollection()
{
	if (vertices != NULL)
	{
		delete vertices;
	}
}

int VertexCollection::add()
{

	return add(vertices);
}

int VertexCollection::add(Vertex vertices[])
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

		assert(index >= 0 && index + verticesPerShape <= numberOfVertices);
	}	

	for(int i = 0; i < verticesPerShape; i++)
	{
		this->vertices[index + i] = vertices[i];
	}
	return index;
}

void VertexCollection::remove(int index)
{
	assert(index >= 0 && index + verticesPerShape <= numberOfVertices);

	for (int i = 0; i < verticesPerShape; i++)
	{
		this->vertices[index + i].position = Vector2f();
	}
	freeSlots.push(index);
}

void VertexCollection::resize(int newSize)
{
	int i;
	Vertex * newArray = new Vertex[newSize * verticesPerShape];

	numberOfVertices = newSize * verticesPerShape;

	for (i = 0; i < size && i < numberOfVertices; i++)
	{
		newArray[i] = vertices[i];
	}
	delete vertices;

	nextIndex = i;
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