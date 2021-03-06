#include"VertexCluster.h"

VertexCluster::~VertexCluster()
{
	for (int i = 0; i < collections.size(); i++)
	{
		delete collections[i];
	}
}

int VertexCluster::create(CollectionTypes type)
{
	return create(type, 1000, 1000);
}

int VertexCluster::create(CollectionTypes type, int size, int increment)
{
	switch (type)
	{
	case VertexCluster::HexagonSource:
		return create(8, size, increment, PrimitiveType::TrianglesStrip);

	case VertexCluster::RectangleSource:
		return create(4, size, increment, PrimitiveType::Quads);

	default:
		assert(false); // Is this better or worse than throwing an exception?

		return -1;
	}
}

int VertexCluster::create(int verticesPerShape, int size, int increment, PrimitiveType type)
{
	int index = collections.size();
	VertexCollection * collection =  new VertexCollection(verticesPerShape, size, increment, type);

	collections.push_back(collection);

	return index;
}

VertexCollection * VertexCluster::getCollection(int index) const
{
	assert(index >= 0 && index < collections.size());

	return collections[index];
}

void VertexCluster::draw(RenderTarget& target, RenderStates states) const
{
	for (int  i = 0; i < collections.size(); i++)
	{
		target.draw(*collections[i], states);
	}
}