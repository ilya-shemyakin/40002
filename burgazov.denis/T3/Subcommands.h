#ifndef SUBCOMMANDS
#define SUBCOMMANDS

#include "Shapes.h"
#include <vector>

double getTriangleArea(const Point& p1, const Point& p2, const Point& p3);
double getPolygonArea(const Polygon& polygon);
//double getAreaOfEven(double areaSum, const Polygon& polygon);
//double getAreaOfOdd(double areaSum, const Polygon& polygon);
double getSumArea(int flag, double areaSum, const Polygon& polygon);
double getMaxArea(double area, const Polygon& polygon);
double getMaxVertexes(size_t vertexes, const Polygon& polygon);
double getMinArea(double area, const Polygon& polygon);
double getMinVertexes(size_t vertexes, const Polygon& polygon);
//bool isEven(const Polygon& polygon);
//bool isOdd(const Polygon& polygon);
bool isVertexes(int flag, const Polygon& polygon);
void getFrame(int& minX, int& maxX, int& minY, int& maxY, const Polygon& polygon);
bool isEqual(const Polygon& left, const Polygon& right);
void subEcho(const Polygon& source, const Polygon& forCompare, std::vector<Polygon>& dest, int& count);

#endif
