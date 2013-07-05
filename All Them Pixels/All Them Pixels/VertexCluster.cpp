#include"VertexCluster.h"

VertexCluster::VertexCluster() : collections1(8, 1000, 1000, PrimitiveType::TrianglesStrip), collections2(4, 2000, 1000, PrimitiveType::Quads)
{
	//collections[HexagonSource] = VertexCollection(8, 200000, PrimitiveType::TrianglesStrip);
	//collections[RectangleSource] = VertexCollection(4, 200000, PrimitiveType::Quads);
}

VertexCollection* VertexCluster::operator[](const int index)
{
	assert(index >= 0 && index < 2);

	if (index == HexagonSource)
	{
		return &collections1;
	}
	else
	{
		return &collections2;
	}
}

void VertexCluster::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(collections1, states);
	target.draw(collections2, states);
}