#include "stdafx.h"
#include "Point.h"

Point::Point()
{
	this->X = 0;
	this->Y = 0;
}

Point::Point(const Point & Other)
{
	this->X = Other.X;
	this->Y = Other.Y;
}

Point::Point(const int X, const int Y)
{
	this->X = X;
	this->Y = Y;
}

Point::~Point()
{
}

bool Point::operator == (Point const & Other)
{
	return this->X == Other.X && this->Y == Other.Y;
}