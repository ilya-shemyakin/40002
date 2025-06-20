#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string>
#include <sstream>

struct Point {
    int x, y;
    bool operator==(Point const& o) const;
    bool operator<(Point const& o) const;
};

struct Polygon {
    std::vector<Point> points;
};

bool readPolygon(std::string const& line, Polygon& poly);
double calculateArea(Polygon const& p);

struct Rect {
    int minx, miny, maxx, maxy;
    Rect(Polygon const& p);
    bool intersects(Rect const& o) const;
};

bool parsePoint(std::string const& tok, Point& pt);

#endif
