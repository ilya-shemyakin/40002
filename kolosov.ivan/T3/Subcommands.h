#ifndef SUBCOMMANDS
#define SUBCOMMANDS

#include "Shapes.h"

namespace subcmd
{
    double getTriangleArea(const shapes::Point& p1,const shapes::Point& p2,const shapes::Point& p3);
    double getPolygonArea(const shapes::Polygon& polygon);
    bool isDigitButBool(char ch);
    double getAreaOfEven(double areaSum,const shapes::Polygon& polygon);
    double getAreaOfOdd(double areaSum, const shapes::Polygon& polygon);
    double getSumArea(double areaSum, const shapes::Polygon& polygon);
    double getVertexesArea(double areaSum, const shapes::Polygon& polygon, const unsigned amountOfVertexes);
    bool comparatorForArea(const shapes::Polygon& left, const shapes::Polygon& right);
    bool comparatorForVertexes(const shapes::Polygon& left, const shapes::Polygon& right);
    bool isEven(const shapes::Polygon& polygon);
    bool isOdd(const shapes::Polygon& polygon);
    bool consistsFromGivenAmountOfVertexes(const shapes::Polygon& polygon,const unsigned amountOfVertexes);
    bool comparatorForX(const shapes::Point& left, const shapes::Point& right);
    bool comparatorForY(const shapes::Point& left, const shapes::Point& right);
    int getMinX(const shapes::Polygon& polygon);
    int getMaxX(const shapes::Polygon& polygon);
    int getMinY(const shapes::Polygon& polygon);
    int getMaxY(const shapes::Polygon& polygon);
    shapes::Point getSide(const shapes::Point& p1, const shapes::Point& p2);
    bool isRightAngle(const shapes::Point& s1, const shapes::Point& s2);
    bool isTrue(bool rule);
    bool hasRightAngle(const shapes::Polygon& polygon);
}

#endif
