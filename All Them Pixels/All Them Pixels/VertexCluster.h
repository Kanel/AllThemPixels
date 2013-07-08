#pragma once

#include "VertexCollection.h"
#include <cassert>
#include <vector>

using std::vector;

class VertexCluster : public Drawable
{
private:
	vector<VertexCollection *> collections;

public:
	enum CollectionTypes
	{
		HexagonSource = 0,
		RectangleSource = 1
	};

public:
	~VertexCluster();

	int create(CollectionTypes type);
	int create(int verticesPerShape, int size, int increment, PrimitiveType type);

	VertexCollection * getCollection(int index);

	virtual void draw(RenderTarget& target, RenderStates states) const;
};