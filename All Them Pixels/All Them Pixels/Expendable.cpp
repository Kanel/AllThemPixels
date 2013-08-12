#include "Expendable.h"

Expendable::Expendable()
{
	_expended = false;
}

bool Expendable::expended()
{
	return _expended;
}

void Expendable::expend()
{
	_expended = true;
}