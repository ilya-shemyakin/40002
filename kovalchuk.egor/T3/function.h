#ifndef FUNCTION_H
#define FUNCTION_H

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <cmath>

struct Point {
    int x;
    int y;
};

struct Polygon {
    std::vector<Point> points;
};

inline bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator==(const Polygon& p1, const Polygon& p2) {
    return p1.points == p2.points;
}

// shoelace formula
inline double polygonArea(const Polygon& p) {
    int n = p.points.size();
    if (n < 3) {
        return 0.0;
    }
    std::vector<Point> cp(p.points);
    cp.push_back(cp.front());
    double crossSum = std::inner_product(
        cp.begin(), cp.end() - 1,
        cp.begin() + 1,
        0.0,
        std::plus<double>(),
        [](const Point& a, const Point& b) {
            return static_cast<double>(a.x) * b.y - static_cast<double>(a.y) * b.x;
        });
    return std::fabs(crossSum) / 2.0;
}


inline bool isRectangle(const Polygon& p) {
    if (p.points.size() != 4) {
        return false;
    }
    Point a{ p.points[1].x - p.points[0].x, p.points[1].y - p.points[0].y };
    Point b{ p.points[2].x - p.points[1].x, p.points[2].y - p.points[1].y };
    Point c{ p.points[3].x - p.points[2].x, p.points[3].y - p.points[2].y };
    Point d{ p.points[0].x - p.points[3].x, p.points[0].y - p.points[3].y };
    int dot_ab = a.x * b.x + a.y * b.y;
    int dot_bc = b.x * c.x + b.y * c.y;
    int dot_cd = c.x * d.x + c.y * d.y;
    int dot_da = d.x * a.x + d.y * a.y;
    return dot_ab == 0 && dot_bc == 0 && dot_cd == 0 && dot_da == 0;
}

inline bool parsePoints(std::istringstream& iss, int count, std::vector<Point>& pts) {
    if (count <= 0) return true;

    char c;
    Point pt;
    if (!(iss >> c) || c != '(') return false;
    if (!(iss >> pt.x)) return false;
    if (!(iss >> c) || c != ';') return false;
    if (!(iss >> pt.y)) return false;
    if (!(iss >> c) || c != ')') return false;

    pts.push_back(pt);
    return parsePoints(iss, count - 1, pts);
}

inline bool parsePolygon(std::istringstream& iss, Polygon& poly) {
    int n;
    if (!(iss >> n) || n < 3) return false;
    poly.points.clear();
    return parsePoints(iss, n, poly.points);
}

inline void readPolygons(std::istream& in, std::vector<Polygon>& polys) {
    std::string line;
    if (!std::getline(in, line)) return;
    std::istringstream iss(line);
    Polygon poly;
    if (parsePolygon(iss, poly)) {
        polys.push_back(poly);
    }
    readPolygons(in, polys);
}

#endif // FUNCTION_H
