#pragma once

#include "Object.h"

/*
	��� ���. �� ����� ���� ������.
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