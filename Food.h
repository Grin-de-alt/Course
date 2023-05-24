#pragma once

#include "Object.h"

/*
	Это еда. Ее будет есть змейка.
*/
class Food : public Object
{

public:
	Food()
	{
		ObjectType = ObjectType::Food;
	}

	virtual char GetSymbolForDisplay() override
	{
		return '+';
	}
};