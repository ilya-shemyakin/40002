#include "Geometry.h"
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iterator>
#include <limits>
#include <cctype>

bool Point::operator==(const Point& o) const
{
    return x == o.x && y == o.y;
}

bool Point::operator<(const Point& o) const
{
    return x < o.x || (x == o.x && y < o.y);
}

bool parsePoint(const std::string& tok, Point& pt)
{
    if (tok.size() < 5 || tok.front() != '(' || tok.back() != ')')
    {
        return false;
    }
    auto s = tok.substr(1, tok.size() - 2);
    auto pos = s.find(';');
    if (pos == std::string::npos)
    {
        return false;
    }
    try
    {
        pt.x = std::stoi(s.substr(0, pos));
        pt.y = std::stoi(s.substr(pos + 1));
    }
    catch (...)
    {
        return false;
    }
    return true;
}

bool readPolygon(const std::string& line, Polygon& poly)
{
    std::istringstream iss(line);
    int n;
    if (!(iss >> n) || n < 3)
    {
        return false;
    }

    poly.points.clear();
    std::vector<std::string> tokens;
    std::copy(std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>(),
        std::back_inserter(tokens));

    if (tokens.size() != static_cast<size_t>(n))
    {
        return false;
    }

    return std::all_of(tokens.begin(), tokens.end(),
        [&poly](const std::string& token)
        {
            Point pt;
            if (!parsePoint(token, pt))
            {
                return false;
            }
            poly.points.push_back(pt);
            return true;
        });
}

double calculateArea(const Polygon& p)
{
    const auto& pts = p.points;
    if (pts.size() < 3)
    {
        return 0.0;
    }

    auto accumulate_area = [](double area, const std::pair<Point, Point>& edge)
        {
            return area + (edge.first.x + edge.second.x) * (edge.first.y - edge.second.y);
        };

    std::vector<std::pair<Point, Point>> edges;
    std::transform(pts.begin(), std::prev(pts.end()),
        std::next(pts.begin()),
        std::back_inserter(edges),
        [](const Point& a, const Point& b)
        {
            return std::make_pair(a, b);
        });

    edges.emplace_back(pts.back(), pts.front());
    double area = std::accumulate(edges.begin(), edges.end(), 0.0, accumulate_area);
    return std::abs(area) * 0.5;
}

Rect::Rect(const Polygon& p)
{
    auto cmpX = [](Point const& a, Point const& b) { return a.x < b.x; };
    auto mmx = std::minmax_element(p.points.begin(), p.points.end(), cmpX);
    auto minXIt = mmx.first;
    auto maxXIt = mmx.second;
    minx = minXIt->x;
    maxx = maxXIt->x;

    auto cmpY = [](Point const& a, Point const& b) { return a.y < b.y; };
    auto mmy = std::minmax_element(p.points.begin(), p.points.end(), cmpY);
    auto minYIt = mmy.first;
    auto maxYIt = mmy.second;
    miny = minYIt->y;
    maxy = maxYIt->y;

}

bool Rect::intersects(const Rect& o) const
{
    return !(maxx < o.minx || o.maxx < minx || maxy < o.miny || o.maxy < miny);
}
