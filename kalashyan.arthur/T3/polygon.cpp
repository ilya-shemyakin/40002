#include "polygon.h"
#include <sstream>
#include <cctype>
#include <numeric>
#include <cmath>

bool readPolygon(const std::string &line, Polygon &poly) {
    std::istringstream iss(line);
    int n;
    if (!(iss >> n) || n < 3) return false;

    poly.points.clear();
    for (int i = 0; i < n; ++i) {
        char ch;
        int x, y;
        if (!(iss >> ch) || ch != '(') return false;
        if (!(iss >> x >> ch) || ch != ';') return false;
        if (!(iss >> y >> ch) || ch != ')') return false;
        poly.points.push_back({x, y});
    }

    std::string rest;
    if (std::getline(iss, rest)) {
        for (char c : rest) {
            if (!isspace(static_cast<unsigned char>(c))) return false;
        }
    }

    return static_cast<int>(poly.points.size()) == n;
}

double calculateArea(const Polygon &p) {
    if (p.points.size() < 3) return 0.0;

    auto it0 = p.points.begin();
    auto it1 = std::next(it0);
    double cross0 = it0->x * p.points.back().y - it0->y * p.points.back().x;

    double sum = std::inner_product(
        it1, p.points.end(), it0, cross0,
        std::plus<>(),
        [](const Point &a, const Point &b) {
            return a.x * b.y - a.y * b.x;
        });

    return std::abs(sum) * 0.5;
}
