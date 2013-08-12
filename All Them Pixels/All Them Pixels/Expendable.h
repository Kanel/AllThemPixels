#pragma once

class Expendable
{
protected:
	bool _expended;

public:
	Expendable();

	virtual bool expended();
	virtual void expend();
};