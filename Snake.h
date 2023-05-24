#pragma once

#include "Object.h"

class Snake : public Object
{
public:
	int X;
	int Y;

public:
	Snake * Head = nullptr;
	Snake * Prev = nullptr;
	Snake * Next = nullptr;

public:
	Snake()
	{
		ObjectType = ObjectType::Snake;
	}

public:
	virtual char GetSymbolForDisplay() override;

};