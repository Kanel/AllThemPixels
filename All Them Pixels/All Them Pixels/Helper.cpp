#include "Helper.h"

int sign(int value)
{
	return (value == 0) ? 0 : ((value < 0) ? -1 : 1);
}

int sign(int value, int i)
{
	return (value == 0) ? 0 : ((value < 0) ? -i : i);
}

int limit(int value, int lower, int upper)
{
	return (value <= lower) ? lower : ((value >= upper) ? upper : value);
}

bool isInRange(int min, int max, int value)
{
	return min <= value && value <= max;
}

int randomInt(int min, int max)
{
	return min + rand() % (max - min);
}

float randomFloat(float min, float max, int decimals)
{
	float div = powf(10, decimals);

	return randomInt(min * div, max * div) / div;
}