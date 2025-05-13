#ifndef COM_H
#define COM_H
#include <iomanip>
#include "geo.h"
#include <numeric>
#include <functional>
#include <algorithm>
#include <string>
#include <set>
#include <iterator>
struct AreaCalculator {
    double operator()(const Polygon& poly) const {
        double area = 0.0;
        int n = poly.points.size();
        for (int i = 0; i < n; ++i) {
            const Point& p1 = poly.points[i];
            const Point& p2 = poly.points[((i + 1) % n)];
            area += (p1.x * p2.y - p2.x * p1.y);
        }
        return std::abs(area) / 2.0;
    }
};

struct PolygonEqual {
    PolygonEqual(const Polygon& p) {
        std::transform(p.points.begin(), p.points.end(),
            std::inserter(set_, set_.begin()),
            [](const Point& p) {
                return p.makeNormal();
            });
    }

    bool operator()(const Polygon& polygon) const {

        if (polygon.points.size() != set_.size())
        {
            return false;
        }

        std::multiset<std::pair<int, int>> curSet;
        std::transform(polygon.points.begin(), polygon.points.end(),
            std::inserter(curSet, curSet.begin()),
            [](const Point& p) {
                return p.makeNormal();
            });

        return curSet == set_;
        // соответсвенно если в одном (1;3), а в другом (1;3) (3;1) то это будет выглядеть как
        // (1;3) и (1;3) (1;3) - не сравняется, все ок
    }
private:
    std::multiset<std::pair<int, int>> set_;
};

bool hasEvenPoints(const Polygon& p) {
    return p.points.size() % 2 == 0;
}

double accumulateOddAreas(double sum, const Polygon& p, AreaCalculator& calc) {
    return !hasEvenPoints(p) ? sum + calc(p) : sum;
}

double accumulateAll(double sum, const Polygon& p, AreaCalculator& calc) {
    return sum + calc(p);
}

double accumulateNumber(double sum, const Polygon& p, AreaCalculator& calc, int number) {
    return (number == p.points.size()) ? sum + calc(p) : sum;
}

bool hasCountVerticales(const Polygon& p, int count) {
    return p.points.size() == count;
}

double accumulateEvenAreas(double sum, const Polygon& p, AreaCalculator& calc) {
    return hasEvenPoints(p) ? sum + calc(p) : sum;
}

double Area(const std::vector<Polygon>& polygons, std::string str) {
    AreaCalculator calc;
    std::cout << std::setprecision(1);
    // используем обёртку вокруг accumulate
    if (str == "EVEN") {
        return std::accumulate(
            polygons.begin(),
            polygons.end(),
            0.0,
            std::bind(accumulateEvenAreas, std::placeholders::_1, std::placeholders::_2, std::ref(calc))
        );
    }
    else if (str == "ODD") {
        return std::accumulate(
            polygons.begin(),
            polygons.end(),
            0.0,
            std::bind(accumulateOddAreas, std::placeholders::_1, std::placeholders::_2, std::ref(calc))
        );
    }
    else if (str == "MEAN") {
        if (polygons.size() > 0) { // иначе делать что-то
            return std::accumulate(
                polygons.begin(),
                polygons.end(),
                0.0,
                std::bind(accumulateAll, std::placeholders::_1, std::placeholders::_2, std::ref(calc))
            ) / polygons.size();
        }
        else {
            return -1;
        }
    }
    else {
        try {
            int number = std::stoi(str); // выкинет исключение если не выйдет
            return std::accumulate(
                polygons.begin(),
                polygons.end(),
                0.0,
                std::bind(accumulateNumber, std::placeholders::_1, std::placeholders::_2, std::ref(calc), number)
            );
        }
        catch (std::exception& e){
            return -1;
        }
    }
}

bool compareByArea(const Polygon& p1, const Polygon& p2, AreaCalculator& calc) {
    return calc(p1) < calc(p2); // Сравниваем площади
}

bool compareByVeg(const Polygon& p1, const Polygon& p2) {
    return p1.points.size() < p2.points.size();
}

double maxSometh(const std::vector<Polygon>& polygons, std::string& str) {
    if (polygons.size() > 0) {
        if (str == "AREA") {
            std::cout << std::setprecision(1);
            AreaCalculator calc;
            auto maxPolygon = std::max_element(polygons.begin(), polygons.end(),
                std::bind(compareByArea, std::placeholders::_1, std::placeholders::_2, std::ref(calc)));
            if (maxPolygon != polygons.end()) { // иначе делать что-то надо
                return calc(*maxPolygon);
            }
        }
        if (str == "VERTEXES") {
            auto maxPolygon = std::max_element(polygons.begin(), polygons.end(), compareByVeg);
            if (maxPolygon != polygons.end()) { // иначе делать что-то надо
                return maxPolygon->points.size();
            }
        }
    }
    return -1;
}
double minSometh(const std::vector<Polygon>& polygons, std::string& str) {
    AreaCalculator calc;
    if (polygons.size() > 0) {
        if (str == "AREA") {
            std::cout << std::setprecision(1);
            auto minPolygon = std::min_element(polygons.begin(), polygons.end(),
                std::bind(compareByArea, std::placeholders::_1, std::placeholders::_2, std::ref(calc)));
            if (minPolygon != polygons.end()) { // иначе делать что-то надо
                return calc(*minPolygon);
            }
        }
        if (str == "VERTEXES") {
            auto minPolygon = std::min_element(polygons.begin(), polygons.end(), compareByVeg);
            if (minPolygon != polygons.end()) { // иначе делать что-то надо
                return minPolygon->points.size();
            }
        }
    };
    return -1;
}

int countSomt(const std::vector<Polygon>& polygons, std::string str) {
    if (str == "EVEN") {
        return std::count_if(polygons.begin(), polygons.end(), hasEvenPoints);
    }
    else if (str == "ODD") {
        return polygons.size() - std::count_if(polygons.begin(), polygons.end(), hasEvenPoints);
    }
    else {
        int number = std::stoi(str); // throw
        return std::count_if(polygons.begin(), polygons.end(), std::bind(hasCountVerticales, std::placeholders::_1, number));
    }
}

bool compareBeCoordPoint(const Point& p1, const Point& p2,const std::string& coor) {
    if (coor == "x") {
        return p1.x < p2.x;
    }
    return p1.y < p2.y;
}

bool compareBeCoordPolygons(const Polygon& p1, const Polygon& p2, const std::string& coor) {
    auto minCoorPoint1 = std::min_element(p1.points.begin(), p1.points.end(),
        std::bind(compareBeCoordPoint, std::placeholders::_1, std::placeholders::_2, coor));
    auto minCoorPoint2 = std::min_element(p2.points.begin(), p2.points.end(),
        std::bind(compareBeCoordPoint, std::placeholders::_1, std::placeholders::_2, coor));
    if (minCoorPoint1 != p1.points.end() && minCoorPoint2 != p2.points.end()) {
        if (coor == "x") {
            return minCoorPoint1->x < minCoorPoint2->x;
        }
        return minCoorPoint1->y < minCoorPoint2->y;
    }
}
// тут получаю крайние точки прямоуга
std::vector<Point> getInframePoints(const std::vector<Polygon>& polygons) {
    auto minXPolygon = std::min_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b) {
            return compareBeCoordPolygons(a, b, "x");
        });
    auto minXPoint = std::min_element(minXPolygon->points.begin(),
        minXPolygon->points.end(),
        [](const Point& a, const Point& b) {
            return compareBeCoordPoint(a, b, "x");
        });

    auto minYPolygon = std::min_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b) {
            return compareBeCoordPolygons(a, b, "y");
        });
    auto minYPoint = std::min_element(minYPolygon->points.begin(),
        minYPolygon->points.end(),
        [](const Point& a, const Point& b) {
            return compareBeCoordPoint(a, b, "y");
        });

    auto maxYPolygon = std::max_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b) {
            return compareBeCoordPolygons(a, b, "y");
        });
    auto maxYPoint = std::max_element(maxYPolygon->points.begin(),
        maxYPolygon->points.end(),
        [](const Point& a, const Point& b) {
            return compareBeCoordPoint(a, b, "y");
        });

    auto maxXPolygon = std::max_element(polygons.begin(), polygons.end(),
        [](const Polygon& a, const Polygon& b) {
            return compareBeCoordPolygons(a, b, "x");
        });
    auto maxXPoint = std::max_element(maxXPolygon->points.begin(),
        maxXPolygon->points.end(),
        [](const Point& a, const Point& b) {
            return compareBeCoordPoint(a, b, "x");
        });

    std::vector<Point> vFrame;
    vFrame.push_back(Point(minXPoint->x, minYPoint->y));
    vFrame.push_back(Point(maxXPoint->x, maxYPoint->y));
    return vFrame;
}

// туту проверяю принадлежит ли точка прямоугу
bool isPointInFrame(const Point& p, const std::vector<Point>& v) {
    return ((v[0].x <= p.x) && (p.x <= v[1].x)) && (v[0].y <= p.y <= v[1].y);
}
// тут мне нужна - фигура искомая и результат getInFrame
// передать надо мой вектор, чтобы получить прямоуг и искомую фигуру
int checkPolygonInFrame(const Polygon& p, const std::vector<Polygon>& vPoly) {
    if (vPoly.empty()) {
        return -1;
    }
    std::vector<Point> vFrame = getInframePoints(vPoly);
    std::cout << "\nграницы: ";
    for (const Point& p : vFrame) {
        std::cout << p << " ";
    }
    std::cout << std::endl;
    int cnt = std::count_if(p.points.begin(), p.points.end(),
        std::bind(isPointInFrame, std::placeholders::_1, vFrame));
    return cnt == p.points.size();
}


int perms(const Polygon& poly, const std::vector<Polygon>& vPoly) {
    return std::count_if(vPoly.begin(), vPoly.end(),
        PolygonEqual(poly));
}

#endif // !COM_H
