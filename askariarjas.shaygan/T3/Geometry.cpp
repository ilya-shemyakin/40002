#include "Geometry.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>
#include <limits>
#include <cctype>

bool Point::operator==(Point const& o) const { return x == o.x && y == o.y; }
bool Point::operator<(Point const& o) const { return x < o.x || (x == o.x && y < o.y); }

bool parsePoint(std::string const& tok, Point& pt) {
    if (tok.size() < 5 || tok.front() != '(' || tok.back() != ')') return false;
    auto s = tok.substr(1, tok.size() - 2);
    auto pos = s.find(';'); if (pos == std::string::npos) return false;
    try { pt.x = std::stoi(s.substr(0, pos)); pt.y = std::stoi(s.substr(pos + 1)); }
    catch (...) { return false; }
    return true;
}

bool readPolygon(std::string const& line, Polygon& poly) {
    std::istringstream iss(line);
    int n; if (!(iss >> n) || n < 3) return false;
    poly.points.clear();
    for (int i = 0;i < n;++i) {
        std::string tok; if (!(iss >> tok)) return false;
        Point pt; if (!parsePoint(tok, pt)) return false;
        poly.points.push_back(pt);
    }
    std::string rest; std::getline(iss, rest);
    return std::all_of(rest.begin(), rest.end(), [](char c) { return std::isspace((unsigned char)c); });
}

double calculateArea(Polygon const& p) {
    auto it0 = p.points.begin();
    auto it1 = std::next(it0);
    double cross0 = it0->x * p.points.back().y - it0->y * p.points.back().x;
    return std::abs(std::inner_product(
        it1, p.points.end(),
        it0, cross0,
        std::plus<>(),
        [](Point const& a, Point const& b) { return a.x * b.y - a.y * b.x; }
    )) * 0.5;
}

Rect::Rect(Polygon const& p)
    : minx(std::numeric_limits<int>::max())
    , miny(std::numeric_limits<int>::max())
    , maxx(std::numeric_limits<int>::lowest())
    , maxy(std::numeric_limits<int>::lowest())
{
    for (auto const& pt : p.points) {
        minx = std::min(minx, pt.x);
        maxx = std::max(maxx, pt.x);
        miny = std::min(miny, pt.y);
        maxy = std::max(maxy, pt.y);
    }
}

bool Rect::intersects(Rect const& o) const {
    return !(maxx < o.minx || o.maxx < minx || maxy < o.miny || o.maxy < miny);
}
