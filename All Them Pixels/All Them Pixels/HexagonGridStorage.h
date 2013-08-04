#pragma once

#include "AxialCoordinates.h"
#include "HexagonGridStorageRow.h"
#include <cassert>

template<class T> class HexagonGridStorage
{
private:
	int layers;
	int offset;
	int size;
	bool loaded;
	HexagonGridStorageRow<T> ** rows;

private:
	void copy(const HexagonGridStorage &that)
	{
		this->layers = that.layers;
		this->offset = that.layers;
		this->loaded = that.loaded;
		this->size = that.size;
		this->rows = new HexagonGridStorageRow<T>*[size];

		for (int i = 0; i < layers * 2 + 1; i++)
		{
			rows[i] = new HexagonGridStorageRow<T>(*that.rows[i]);
		}
	}

public:
	HexagonGridStorage(const HexagonGridStorage &that)
	{
		copy(that);
	}

	HexagonGridStorage(int layers)
	{
		int index = 0;

		this->layers = layers;
		this->offset = layers;
		this->loaded = true;
		this->size = layers * 2 + 1;
		this->rows = new HexagonGridStorageRow<T>*[size];

		for (int i = layers; i > 0; i--)
		{
			rows[index++] = new HexagonGridStorageRow<T>(size - i, offset, i);
		}
		rows[index++] = new HexagonGridStorageRow<T>(size, offset);

		for (int i = 1; i <= layers; i++)
		{
			rows[index++] = new HexagonGridStorageRow<T>(size - i, offset);
		}
	}

	HexagonGridStorage()
	{
		this->layers = 0;
		this->offset = 0;
		this->loaded = false;
		this->size = 0;
		this->rows = new HexagonGridStorageRow<T>*[size];
	}

	~HexagonGridStorage()
	{
		for (int i = 0; i < size; i++)
		{
			delete rows[i];
		}
		delete[] rows;
	}

	bool contains(int q, int r)
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

	bool contains(AxialCoordinates coordinates)
	{
		return contains(coordinates.q, coordinates.r);
	}

	bool isLoaded()
	{
		return loaded;
	}

	void load()
	{
		assert(false);
	}

	void unload()
	{
		assert(false);
	}

	HexagonGridStorageRow<T> & operator[](const int index) const
	{
		assert(isInRange(0, size - 1, offset + index) && loaded && "Invalid index!");

		return *rows[offset + index];
	}

	T &  operator[](const AxialCoordinates index) const
	{
		assert(isInRange(0, size - 1, offset + index.q) && loaded && "Invalid index!");

		return (*rows[offset + index.q])[index.r];
	}

	HexagonGridStorage & operator=(const HexagonGridStorage &that)
	{
		if (this != &that)
		{
			this->~HexagonGridStorage();
			this->copy(that);
		}
		return *this;
	}
};
