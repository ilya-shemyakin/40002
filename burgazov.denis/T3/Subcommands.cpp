#include "Subcommands.h"

#include <cmath>
#include <vector>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <functional>

double getTriangleArea(const Point& p1, const Point& p2, const Point& p3)
{
    return 0.5 * std::abs((p1.x - p3.x) * (p2.y - p1.y) - (p1.x - p2.x) * (p3.y - p1.y));
}

double getPolygonArea(const Polygon& polygon)
{
    std::vector< double > triangleAreas(polygon.points.size() - 2);
    std::transform
    (
        polygon.points.begin() + 2, polygon.points.end(),
        polygon.points.begin() + 1, triangleAreas.begin(),
        std::bind(getTriangleArea, std::ref(polygon.points[0]), std::placeholders::_1, std::placeholders::_2)
    );
    return std::accumulate(triangleAreas.cbegin(), triangleAreas.cend(), 0.0);
}

double getAreaOfEven(double areaSum, const Polygon& polygon)
{
    if (polygon.points.size() % 2 == 0)
    {
        return areaSum + getPolygonArea(polygon);
    }
    return areaSum;
}

double getAreaOfOdd(double areaSum, const Polygon& polygon)
{
    if (polygon.points.size() % 2 != 0)
    {
        return areaSum + getPolygonArea(polygon);
    }
    return areaSum;
}

double getSumArea(double areaSum, const Polygon& polygon)
{
    return areaSum + getPolygonArea(polygon);
}

double getVertexesArea(double areaSum, const Polygon& polygon, const unsigned countOfVertexes)
{
    if (polygon.points.size() == countOfVertexes)
    {
        return areaSum + getPolygonArea(polygon);
    }
    return areaSum;
}

double getMaxArea(double area, const Polygon& polygon) {
    return std::max(area, getPolygonArea(polygon));
}

double getMaxVertexes(const size_t vertexes, const Polygon& polygon) {
    return std::max(vertexes, polygon.points.size());
}

double getMinArea(double area, const Polygon& polygon) {
    return std::min(area, getPolygonArea(polygon));
}

double getMinVertexes(const size_t vertexes, const Polygon& polygon) {
    return std::min(vertexes, polygon.points.size());
}

bool isEven(const Polygon& polygon)
{
    return polygon.points.size() % 2 == 0;
}

bool isOdd(const Polygon& polygon)
{
    return polygon.points.size() % 2 != 0;
}
bool isVertexes(const Polygon& polygon, const unsigned countOfVertexes) {
    return polygon.points.size() == countOfVertexes;
}

bool comparatorX(const Point& left, const Point& right)
{
    return left.x < right.x;
}

bool comparatorY(const Point& left, const Point& right)
{
    return left.y < right.y;
}

void getFrame(int& minX, int& maxX, int& minY, int& maxY, const Polygon& polygon) {
    minX = std::min(minX, (*std::min_element(polygon.points.cbegin(), polygon.points.cend(), comparatorX)).x);
    maxX = std::max(maxX, (*std::max_element(polygon.points.cbegin(), polygon.points.cend(), comparatorX)).x);
    minY = std::min(minY, (*std::min_element(polygon.points.cbegin(), polygon.points.cend(), comparatorY)).y);
    maxY = std::max(maxY, (*std::max_element(polygon.points.cbegin(), polygon.points.cend(), comparatorY)).y);
}

bool predicatePoint(const Point& left, const Point& right) {
    return left.x == right.x && left.y == right.y;
}

bool isEqual(const Polygon& left, const Polygon& right) {
    if (left.points.size() != right.points.size()) {
        return false;
    }
    return std::equal(left.points.begin(), left.points.end(), right.points.begin(), predicatePoint);
}

void subEcho(const Polygon& source, const Polygon& forCompare, std::vector<Polygon>& dest, int& count) {
    if (isEqual(source, forCompare)) {
        dest.push_back(source);
        count++;
    }
    dest.push_back(source);
}
