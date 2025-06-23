#ifndef POLYGON_H
#define POLYGON_H

#include <vector>
#include <string>
#include "point.h"

struct Polygon {
    std::vector<Point> points;
};

bool readPolygon(const std::string &line, Polygon &poly);
double calculateArea(const Polygon &p);

#endif // POLYGON_H
