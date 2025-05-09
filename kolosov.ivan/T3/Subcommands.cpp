#include "Subcommands.h"

#include <cmath>
#include <vector>
#include <numeric>
#include <iterator>
#include <algorithm>
#include <functional>

namespace subcmd
{
    double getTriangleArea(const shapes::Point& p1, const shapes::Point& p2, const shapes::Point& p3)
    {
        return 0.5 * std::abs((p1.x - p3.x) * (p2.y - p1.y) - (p1.x - p2.x) * (p3.y - p1.y));
    }

    double getPolygonArea(const shapes::Polygon& polygon)
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

    double getAreaOfEven(double areaSum,const shapes::Polygon& polygon)
    {
        if (polygon.points.size() % 2 == 0)
        {
            return areaSum + getPolygonArea(polygon);
        }
        return areaSum;
    }

    double getAreaOfOdd(double areaSum, const shapes::Polygon& polygon)
    {
        if (polygon.points.size() % 2 != 0)
        {
            return areaSum + getPolygonArea(polygon);
        }
        return areaSum;
    }

    double getSumArea(double areaSum, const shapes::Polygon& polygon)
    {
        return areaSum + getPolygonArea(polygon);
    }

    double getVertexesArea(double areaSum, const shapes::Polygon& polygon, const int amountOfVertexes)
    {
        if (polygon.points.size() == amountOfVertexes)
        {
            return areaSum + getPolygonArea(polygon);
        }
        return areaSum;
    }

    bool comparatorForArea(const shapes::Polygon& left, const shapes::Polygon& right)
    {
        return getPolygonArea(left) < getPolygonArea(right);
    }

    bool comparatorForVertexes(const shapes::Polygon& left, const shapes::Polygon& right)
    {
        return left.points.size() < right.points.size();
    }

    bool isEven(const shapes::Polygon& polygon)
    {
        return polygon.points.size() % 2 == 0;
    }

    bool isOdd(const shapes::Polygon& polygon)
    {
        return polygon.points.size() % 2 != 0;
    }

    bool consistsFromGivenAmountOfVertexes(const shapes::Polygon& polygon, int amountOfVertexes)
    {
        return polygon.points.size() == amountOfVertexes;
    }

    bool comparatorForX(const shapes::Point& left, const shapes::Point& right)
    {
        return left.x < right.x;
    }

    bool comparatorForY(const shapes::Point& left, const shapes::Point& right)
    {
        return left.y < right.y;
    }

    int getMinX(const shapes::Polygon& polygon)
    {
        return (*std::min_element(polygon.points.cbegin(), polygon.points.cend(), comparatorForX)).x;
    }

    int getMaxX(const shapes::Polygon& polygon)
    {
        return (*std::max_element(polygon.points.cbegin(), polygon.points.cend(), comparatorForX)).x;
    }

    int getMinY(const shapes::Polygon& polygon)
    {
        return (*std::min_element(polygon.points.cbegin(), polygon.points.cend(), comparatorForY)).y;
    }

    int getMaxY(const shapes::Polygon& polygon)
    {
        return (*std::max_element(polygon.points.cbegin(), polygon.points.cend(), comparatorForY)).y;
    }

    shapes::Point& getSide(const shapes::Point& p1, const shapes::Point& p2)
    {
        shapes::Point side;
        side.x = p2.x - p1.x;
        side.y = p2.y - p1.y;
        return side;
    }

    bool isRightAngle(const shapes::Point& s1, const shapes::Point& s2)
    {
        return s1.x * s2.x + s1.y * s2.y == 0;
    }

    bool isTrue(bool rule)
    {
        return rule == true;
    }

    bool hasRightAngle(const shapes::Polygon& polygon)
    {
        std::vector< shapes::Point > sides(polygon.points.size());
        std::transform
        (
            polygon.points.begin() + 1, polygon.points.end(), polygon.points.begin(),
            sides.begin(), getSide
        );
        sides[polygon.points.size() - 1] = getSide(polygon.points[0], polygon.points[polygon.points.size() - 1]);
        std::vector< bool > isRightAngleVector(polygon.points.size());
        std::transform
        (
            sides.begin() + 1, sides.end(), sides.begin(),
            isRightAngleVector.begin(), isRightAngle
        );
        isRightAngleVector[polygon.points.size() - 1] = isRightAngle(sides[0], sides[polygon.points.size() - 1]);

        return std::any_of(isRightAngleVector.begin(), isRightAngleVector.end(), isTrue);
    }
}
