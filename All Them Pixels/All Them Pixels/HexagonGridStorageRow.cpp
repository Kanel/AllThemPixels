#include "HexagonGridStorageRow.h"

void HexagonGridStorageRow::copy(const HexagonGridStorageRow &that)
{
	this->size = that.size;
	this->offset = that.offset;
	this->padding = that.padding;
	this->hexagons = new Hexagon*[size];
	this->loaded = that.loaded;

	for (int i = 0; i < size; i++)
	{
		hexagons[i] = that.hexagons[i];
	}
}

HexagonGridStorageRow::HexagonGridStorageRow(const HexagonGridStorageRow &that)
{
	copy(that);
}

HexagonGridStorageRow::HexagonGridStorageRow(int size, int offset, int padding)
{
	this->size = size;
	this->offset = offset;
	this->padding = padding;
	this->hexagons = new Hexagon*[size];
	this->loaded = true;
}

HexagonGridStorageRow::~HexagonGridStorageRow()
{
	delete[] hexagons;
}

bool HexagonGridStorageRow::isLoaded()
{
	return loaded;
}

void HexagonGridStorageRow::load()
{
	assert(false);
}

void HexagonGridStorageRow::unload()
{
	assert(false);
}

Hexagon *& HexagonGridStorageRow::operator[](const int index)
{
	assert(isInRange(0, size - 1, offset + index - padding) && loaded && "Invalid index!");

	return hexagons[offset + index - padding];
}

HexagonGridStorageRow & HexagonGridStorageRow::operator=(const HexagonGridStorageRow &that)
{
	if (this != &that)
	{
		this->~HexagonGridStorageRow();
		this->copy(that);
	}
	return *this;
}