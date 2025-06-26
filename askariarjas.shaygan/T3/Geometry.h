#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string>
#include <sstream>

struct Point {
    int x, y;
    bool operator==(const Point& o) const;
    bool operator<(const Point& o) const;
};

struct Polygon {
    std::vector<Point> points;
};

bool readPolygon(const std::string& line, Polygon& poly);
double calculateArea(const Polygon& p);

struct Rect {
    int minx, miny, maxx, maxy;
    Rect(const Polygon& p);
    bool intersects(const Rect& o) const;
};

bool parsePoint(const std::string& tok, Point& pt);

#endif
