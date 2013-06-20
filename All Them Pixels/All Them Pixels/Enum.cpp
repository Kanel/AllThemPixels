#include "Enum.h"

bool Enum::isFlagSet(int value, int flag)
{
	return (value & flag) == flag;
}