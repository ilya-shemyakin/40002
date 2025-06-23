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

bool hasEvenCountPoints(const Polygon& p) {
    return p.points.size() % 2 == 0;
}
// odd - нечет
double accumulateOddAreas(double sum, const Polygon& p, AreaCalculator& calc) {
    return !hasEvenCountPoints(p) ? sum + calc(p) : sum;
}

double accumulateAll(double sum, const Polygon& p, AreaCalculator& calc) {
    return sum + calc(p);
}

double accumulateNumber(double sum, const Polygon& p, AreaCalculator& calc, int number) {
    return (static_cast<size_t>(number) == p.points.size()) ? sum + calc(p) : sum;
    // я ранее выъожу из фунции, если number < 0
}

bool hasCountVerticales(const Polygon& p, int count) {
    return p.points.size() == static_cast<size_t>(count);
}

double accumulateEvenAreas(double sum, const Polygon& p, AreaCalculator& calc) {
    return hasEvenCountPoints(p) ? sum + calc(p) : sum;
}

double Area(const std::vector<Polygon>& polygons, std::string str) {
    AreaCalculator calc;
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
        ); // передаем соответсвенно накопленную сумму (которая с 0 шла) и текущий элемент
    }
    else if (str == "MEAN") {
        if (polygons.size() > 0) {
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
            if (number <= 2) {
                return -1;
            }
            return std::accumulate(
                polygons.begin(),
                polygons.end(),
                0.0,
                std::bind(accumulateNumber, std::placeholders::_1, std::placeholders::_2, std::ref(calc), number)
            );
        }
        catch (std::exception& e) {
            return -1;
        }
    }
}

bool compareByArea(const Polygon& p1, const Polygon& p2, AreaCalculator& calc) {
    return calc(p1) < calc(p2);
}

bool compareByVeg(const Polygon& p1, const Polygon& p2) {
    return p1.points.size() < p2.points.size();
}

double maxSometh(const std::vector<Polygon>& polygons, std::string& str) {
    if (polygons.size() > 0) {
        if (str == "AREA") {
            AreaCalculator calc;
            auto maxPolygon = std::max_element(polygons.begin(), polygons.end(),
                std::bind(compareByArea, std::placeholders::_1, std::placeholders::_2, std::ref(calc)));
            return calc(*maxPolygon);
        }
        if (str == "VERTEXES") {
            auto maxPolygon = std::max_element(polygons.begin(), polygons.end(), compareByVeg);
            return maxPolygon->points.size();
        }
    }
    return -1;
}
double minSometh(const std::vector<Polygon>& polygons, std::string& str) {
    AreaCalculator calc;
    if (polygons.size() > 0) {
        if (str == "AREA") {
            auto minPolygon = std::min_element(polygons.begin(), polygons.end(),
                std::bind(compareByArea, std::placeholders::_1, std::placeholders::_2, std::ref(calc)));
            return calc(*minPolygon);
        }
        if (str == "VERTEXES") {
            auto minPolygon = std::min_element(polygons.begin(), polygons.end(), compareByVeg);
            return minPolygon->points.size();
        }
    }
    return -1;
}

int countSomt(const std::vector<Polygon>& polygons, std::string str) {
    if (str == "EVEN") {
        return std::count_if(polygons.begin(), polygons.end(), hasEvenCountPoints);
    }
    else if (str == "ODD") {
        return polygons.size() - std::count_if(polygons.begin(), polygons.end(), hasEvenCountPoints);
    }
    else {
        try {
            int number = std::stoi(str);
            if (number <= 2) {
                return -1;
            }
            return std::count_if(polygons.begin(), polygons.end(), std::bind(hasCountVerticales, std::placeholders::_1, number));
        }
        catch (std::exception& e) {
            return -1;
        }
    }
}

std::vector<Point> getInframePoints(const std::vector<Polygon>& polygons) {
    std::vector<Point> allPoints;
    std::accumulate(polygons.begin(), polygons.end(), 0,
        [&allPoints](int, const Polygon& poly) {
            std::copy(poly.points.begin(), poly.points.end(), std::back_inserter(allPoints));
            return 0;
        });

    auto minX = std::min_element(allPoints.begin(), allPoints.end(),
        [](const Point& a, const Point& b) { return a.x < b.x; });
    auto maxX = std::max_element(allPoints.begin(), allPoints.end(),
        [](const Point& a, const Point& b) { return a.x < b.x; });

    auto minY = std::min_element(allPoints.begin(), allPoints.end(),
        [](const Point& a, const Point& b) { return a.y < b.y; });
    auto maxY = std::max_element(allPoints.begin(), allPoints.end(),
        [](const Point& a, const Point& b) { return a.y < b.y; });

    std::vector<Point> vFrame;
    vFrame.push_back(Point(minX->x, minY->y));  // нижний левый угол
    vFrame.push_back(Point(maxX->x, maxY->y));  // верхний правый угол
    return vFrame;
}

// туту проверяю принадлежит ли точка прямоугу
bool isPointInFrame(const Point& p, const std::vector<Point>& v) {
    return (v[0].x <= p.x) && (p.x <= v[1].x) && (v[0].y <= p.y) && (p.y <= v[1].y);
}
// тут мне нужна - фигура искомая и результат getInFrame
// передать надо мой вектор, чтобы получить прямоуг и искомую фигуру
int checkPolygonInFrame(const Polygon& p, const std::vector<Polygon>& vPoly) {
    if (vPoly.empty() || p.points.size() <= 2) {
        return -1;
    }
    std::vector<Point> vFrame = getInframePoints(vPoly);
    /* std::cout << "\nграницы: ";
     for (const Point& p : vFrame) {
         std::cout << p << " ";
     }
     std::cout << std::endl;*/
    int cnt = std::count_if(p.points.begin(), p.points.end(),
        std::bind(isPointInFrame, std::placeholders::_1, vFrame));
    return cnt == static_cast<int>(p.points.size());
}


int perms(const Polygon& poly, const std::vector<Polygon>& vPoly) {
    if (poly.points.size() <= 2) {
        return -1;
    }
    return std::count_if(vPoly.begin(), vPoly.end(),
        PolygonEqual(poly));
}

#endif // !COM_H
