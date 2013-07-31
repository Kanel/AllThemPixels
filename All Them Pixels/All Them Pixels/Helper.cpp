int sign(int value)
{
	return (value == 0) ? 0 : ((value < 0) ? -1 : 1);
}

int limit(int value, int lower, int upper)
{
	return (value <= lower) ? lower : ((value >= upper) ? upper : value);
}