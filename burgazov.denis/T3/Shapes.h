#ifndef SHAPES_H
#define SHAPES_H

#include <iostream>
#include <vector>

struct DelimiterIO
{
    char exp;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);

struct Point
{
    int x, y;
};

std::istream& operator>>(std::istream& in, Point& point);

struct Polygon
{
    std::vector<Point> points;
};

std::istream& operator>>(std::istream& in, Polygon& polygon);

#endif
