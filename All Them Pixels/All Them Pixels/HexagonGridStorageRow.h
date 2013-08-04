#pragma once

#include "Hexagon.h"
#include "Helper.h"

class HexagonGridStorageRow
{
private:
	int padding;
	int size;
	int offset;
	bool loaded;
	Hexagon ** hexagons;

private:
	void copy(const HexagonGridStorageRow &that);

public:
	HexagonGridStorageRow(const HexagonGridStorageRow &that);
	HexagonGridStorageRow(int size, int offset, int padding = 0);
	~HexagonGridStorageRow();

	bool isLoaded();
	void load();
	void unload();

	Hexagon *& operator[](const int index);
	HexagonGridStorageRow & operator=(const HexagonGridStorageRow &that);
};
