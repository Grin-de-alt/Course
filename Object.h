#pragma once

#include "Enums.h"

class Object
{

public:
	ObjectType ObjectType;

public:
	virtual char GetSymbolForDisplay() = 0;
};