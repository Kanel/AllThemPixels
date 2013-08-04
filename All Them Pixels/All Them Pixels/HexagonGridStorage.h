#pragma once

#include "HexagonGridStorageRow.h"
#include "AxialCoordinates.h"

class HexagonGridStorage
{
private:
	int layers;
	int offset;
	int size;
	bool loaded;
	HexagonGridStorageRow ** rows;

private:
	void copy(const HexagonGridStorage &that);

public:
	HexagonGridStorage(const HexagonGridStorage &that);
	HexagonGridStorage(int layers);
	HexagonGridStorage();
	~HexagonGridStorage();

	bool contains(int q, int r);
	bool contains(AxialCoordinates coordinates);

	bool isLoaded();
	void load();
	void unload();

	HexagonGridStorageRow & operator[](const int index);
	Hexagon *&  operator[](const AxialCoordinates index);
	HexagonGridStorage & operator=(const HexagonGridStorage &that);
};
