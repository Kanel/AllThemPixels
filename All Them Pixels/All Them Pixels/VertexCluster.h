#pragma once

#include "VertexCollection.h"
#include <cassert>
#include <vector>

using std::vector;

// VertexCluster provides a waycreating, storing and managing VertexCollections.
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

	// Creates a default VertexCollection of the cpecified type.
	// returns: The index of the created collection.
	int create(CollectionTypes type);

	// Creates a VertexCollection with the specified settings.
	// verticesPerShape:
	// size: The number of slots for shapes the collection can store.
	// increment: The number of slots the container will grow by once it's full.
	// type: The primitive type to use when drawing.
	// returns: The index of the created collection.
	int create(int verticesPerShape, int size, int increment, PrimitiveType type);

	// index: The index of the collection to retrive.
	// returns: The collection associated with the index.
	VertexCollection * getCollection(int index);

	virtual void draw(RenderTarget& target, RenderStates states) const;
};