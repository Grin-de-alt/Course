#pragma once

#include "stdafx.h"
#include "Object.h"

class Wall : public Object
{
public:
	Wall()
	{
		ObjectType = ObjectType::Wall;
	}

public:
	char GetSymbolForDisplay() override
	{
		return '*';
	}
};