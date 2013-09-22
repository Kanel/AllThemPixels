#pragma once
#include <stdlib.h>
#include <math.h>

int sign(int value);
int sign(int value, int i);
int limit(int value, int lower, int upper);
bool isInRange(int min, int max, int value);
int randomInt(int min, int max);
float randomFloat(float min, float max, int decimals);