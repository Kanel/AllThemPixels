#pragma once

#include "VertexCollection.h"
#include <cassert>

class VertexCluster : public Drawable
{
private:
	VertexCollection collections1;
	VertexCollection collections2;

public:
	enum Sources
	{
		HexagonSource = 0,
		RectangleSource = 1
	};

public:
	VertexCluster();

	VertexCollection* operator[](const int index);

	virtual void draw(RenderTarget& target, RenderStates states) const;
};