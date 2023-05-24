#pragma once

class Point
{
public:
	int X, Y;

public:
	bool operator == (Point const &);

public:
	Point();
	Point(const Point &);
	Point(const int, const int);
	~Point();
};
