#pragma once

#include "Helper.h"
#include <cassert>
#include <type_traits>

template<class T> class HexagonGridStorageRow
{
private:
	int padding;
	int size;
	int offset;
	bool loaded;
	T * items;

private:
	void copy(const HexagonGridStorageRow &that)
	{
		this->size = that.size;
		this->offset = that.offset;
		this->padding = that.padding;
		this->items = new T[size];
		this->loaded = that.loaded;

		for (int i = 0; i < size; i++)
		{
			items[i] = that.items[i];
		}
	}

public:
	HexagonGridStorageRow(const HexagonGridStorageRow &that)
	{
		copy(that);
	}

	HexagonGridStorageRow(int size, int offset, int padding = 0)
	{
		this->size = size;
		this->offset = offset;
		this->padding = padding;
		this->items = new T[size];
		this->loaded = true;
	}

	~HexagonGridStorageRow()
	{
		delete[] items;
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

	T & operator[](const int index) const
	{
		assert(isInRange(0, size - 1, offset + index - padding) && loaded && "Invalid index!");

		return items[offset + index - padding];
	}

	HexagonGridStorageRow & operator=(const HexagonGridStorageRow &that)
	{
		if (this != &that)
		{
			this->~HexagonGridStorageRow();
			this->copy(that);
		}
		return *this;
	}
};

// All this just to set pointers to NULL...
template<class T> class HexagonGridStorageRow<T*>
{
private:
	int padding;
	int size;
	int offset;
	bool loaded;
	T ** items;

private:
	void copy(const HexagonGridStorageRow &that)
	{
		this->size = that.size;
		this->offset = that.offset;
		this->padding = that.padding;
		this->items = new T*[size];
		this->loaded = that.loaded;

		for (int i = 0; i < size; i++)
		{
			items[i] = that.items[i];
		}
	}

public:
	HexagonGridStorageRow(const HexagonGridStorageRow &that)
	{
		copy(that);
	}

	HexagonGridStorageRow(int size, int offset, int padding = 0)
	{
		this->size = size;
		this->offset = offset;
		this->padding = padding;
		this->items = new T*[size];
		this->loaded = true;

		memset(items, NULL, size * sizeof(T*));
	}

	~HexagonGridStorageRow()
	{
		delete[] items;
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

	T* & operator[](const int index) const
	{
		assert(isInRange(0, size - 1, offset + index - padding) && loaded && "Invalid index!");

		return items[offset + index - padding];
	}

	HexagonGridStorageRow & operator=(const HexagonGridStorageRow &that)
	{
		if (this != &that)
		{
			this->~HexagonGridStorageRow();
			this->copy(that);
		}
		return *this;
	}
};