#ifndef SHAPES_H
#define SHAPES_H

#include <iostream>
#include <iomanip>
#include <vector>

namespace shapes
{

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
}

#endif
