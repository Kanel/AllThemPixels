#include "Flag.h"

bool Flag::isFlagSet(int value, int flag)
{
	return (value & flag) == flag;
}