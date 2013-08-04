#include "HexagonGridStorage.h"

void HexagonGridStorage::copy(const HexagonGridStorage &that)
{
	this->layers = that.layers;
	this->offset = that.layers;
	this->loaded = that.loaded;
	this->size = that.size;
	this->rows = new HexagonGridStorageRow*[size];

	for (int i = 0; i < layers * 2 + 1; i++)
	{
		rows[i] = new HexagonGridStorageRow(*that.rows[i]);
	}
}

HexagonGridStorage::HexagonGridStorage(const HexagonGridStorage &that)
{
	copy(that);
}

HexagonGridStorage::HexagonGridStorage(int layers)
{
	int index = 0;

	this->layers = layers;
	this->offset = layers;
	this->loaded = true;
	this->size = layers * 2 + 1;
	this->rows = new HexagonGridStorageRow*[size];

	for (int i = layers; i > 0; i--)
	{
		rows[index++] = new HexagonGridStorageRow(size - i, offset, i);
	}
	rows[index++] = new HexagonGridStorageRow(size, offset);

	for (int i = 1; i <= layers; i++)
	{
		rows[index++] = new HexagonGridStorageRow(size - i, offset);
	}
}

HexagonGridStorage::HexagonGridStorage()
{
	this->layers = 0;
	this->offset = 0;
	this->loaded = false;
	this->size = 0;
	this->rows = new HexagonGridStorageRow*[size];
}

HexagonGridStorage::~HexagonGridStorage()
{
	for (int i = 0; i < size; i++)
	{
		delete rows[i];
	}
	delete[] rows;
}

bool HexagonGridStorage::contains(int q, int r)
{
	if (abs(q) > offset || abs(r) > offset)
	{
		return false;
	}
	else if (q < 0)
	{
		return r >= -offset - q;
	}
	else if (q > 0)
	{
		return r <= offset - q;
	}
	else
	{
		return true;
	}
}

bool HexagonGridStorage::contains(AxialCoordinates coordinates)
{
	return contains(coordinates.q, coordinates.r);
}

bool HexagonGridStorage::isLoaded()
{
	return loaded;
}

void HexagonGridStorage::load()
{
	assert(false);
}

void HexagonGridStorage::unload()
{
	assert(false);
}

HexagonGridStorageRow & HexagonGridStorage::operator[](const int index)
{
	assert(isInRange(0, size - 1, offset + index) && loaded && "Invalid index!");

	return *rows[offset + index];
}

Hexagon *& HexagonGridStorage::operator[](const AxialCoordinates index)
{
	assert(isInRange(0, size - 1, offset + index.q) && loaded && "Invalid index!");

	return (*rows[offset + index.q])[index.r];
}

HexagonGridStorage & HexagonGridStorage::operator=(const HexagonGridStorage &that)
{
	if (this != &that)
	{
		this->~HexagonGridStorage();
		this->copy(that);
	}
	return *this;
}