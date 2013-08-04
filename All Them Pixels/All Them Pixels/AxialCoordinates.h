#pragma once

class AxialCoordinates
{
public:
	int q;
	int r;

public:
	AxialCoordinates() 
	{
		this->q = 0;
		this->r = 0;
	}

	AxialCoordinates(int q, int r)
	{
		this->q = q;
		this->r = r;
	}

	AxialCoordinates operator+(AxialCoordinates &other)
	{
		return AxialCoordinates(q + other.q, r + other.r);
	}
};