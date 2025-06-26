#pragma once
#ifndef FUNCTION_H
#define FUNCTION_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iomanip>

class InvalidCommandException : public std::exception {
public:
    const char* what() const noexcept override {
        return "<INVALID COMMAND>";
    }
};

struct Point {
    int x, y;
};

struct Polygon {
    std::vector<Point> points;
};


double polygonArea(const Polygon& poly)
{
    if (poly.points.size() < 3)
        return 0.0;
    long long area2 = 0;
    for (size_t i = 0; i < poly.points.size(); ++i)
    {
        const Point& p1 = poly.points[i];
        const Point& p2 = poly.points[(i + 1) % poly.points.size()];
        area2 += static_cast<long long>(p1.x) * p2.y - static_cast<long long>(p2.x) * p1.y;
    }
    return std::abs(area2) / 2.0;
}

bool isPerpendicular(const Point& v1, const Point& v2)
{
    return (v1.x * v2.x + v1.y * v2.y) == 0;
}
bool isRectangle(const Polygon& poly)
{
    if (poly.points.size() != 4)
        return false;
    std::vector<Point> vectors
    {
        Point{ poly.points[1].x - poly.points[0].x, poly.points[1].y - poly.points[0].y },
            Point{ poly.points[2].x - poly.points[1].x, poly.points[2].y - poly.points[1].y },
            Point{ poly.points[3].x - poly.points[2].x, poly.points[3].y - poly.points[2].y },
            Point{ poly.points[0].x - poly.points[3].x, poly.points[0].y - poly.points[3].y }
    };

    return
        isPerpendicular(vectors[0], vectors[1]) &&
        isPerpendicular(vectors[1], vectors[2]) &&
        isPerpendicular(vectors[2], vectors[3]) &&
        isPerpendicular(vectors[3], vectors[0]);
}


std::vector<Polygon> readPolygons(std::ifstream& fin)
{
    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(fin, line))
    {
        if (line.empty())
            continue;
        std::istringstream iss(line);
        int n;
        if (!(iss >> n) || n < 3)
            continue;
        Polygon poly;
        bool valid = true;
        for (int i = 0; i < n; ++i) {
            char c;
            int x, y;
            if (!(iss >> c) || c != '(' || !(iss >> x) ||
                !(iss >> c) || c != ';' || !(iss >> y) ||
                !(iss >> c) || c != ')')
            {
                valid = false;
                break;
            }
            poly.points.emplace_back(Point{ x, y });
        }
        std::string extra;
        if (valid && !(iss >> extra))
        {
            polygons.push_back(poly);
        }
    }
    return polygons;
}

bool hasNoMoreArguments(std::istringstream& iss)
{
    iss >> std::ws;
    return iss.eof();
}


void handleArea(std::istringstream& iss, const std::vector<Polygon>& polygons, std::ostream& os = std::cout)
{
    std::string arg;
    if (!(iss >> arg))
    {
        throw InvalidCommandException();
        return;
    }
    if (arg == "EVEN" || arg == "ODD")
    {
        if (!hasNoMoreArguments(iss))
        {
            throw InvalidCommandException();
            return;
        }
        bool wantEven = (arg == "EVEN");
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [wantEven](double acc, const Polygon& p)
            {
                bool isEven = (p.points.size() % 2 == 0);
                return acc + (isEven == wantEven ? polygonArea(p) : 0);
            });
        os << sum << std::endl;
    }
    else if (arg == "MEAN") {
        if (!hasNoMoreArguments(iss))
        {
            throw InvalidCommandException();
            return;
        }
        if (polygons.empty())
            os << "0.0" << std::endl;
        else
        {
            double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [](double acc, const Polygon& p) { return acc + polygonArea(p); });
            os << (total / polygons.size()) << std::endl;
        }
    }
    else
    {
        bool isNum = std::all_of(arg.begin(), arg.end(), ::isdigit);
        if (!isNum || !hasNoMoreArguments(iss))
        {
            throw InvalidCommandException();
            return;
        }
        int num = std::stoi(arg);
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [num](double acc, const Polygon& p) {
                return acc + (p.points.size() == num ? polygonArea(p) : 0);
            });
        os << sum << std::endl;
    }
}


void handleExtremum(std::istringstream& iss, const std::vector<Polygon>& polygons, std::ostream& os, bool isMax) {
    std::string arg;
    if (!(iss >> arg) || !hasNoMoreArguments(iss)) {
        throw InvalidCommandException();;
        return;
    }
    if (arg == "AREA") {
        if (polygons.empty()) {
            os << "0.0" << std::endl;
        }
        else {
            auto compareArea = [](const Polygon& a, const Polygon& b) {
                return polygonArea(a) < polygonArea(b);
                };
            if (isMax) {
                auto it = std::max_element(polygons.begin(), polygons.end(), compareArea);
                os << polygonArea(*it) << std::endl;
            }
            else {
                auto it = std::min_element(polygons.begin(), polygons.end(), compareArea);
                os << polygonArea(*it) << std::endl;
            }
        }
    }
    else if (arg == "VERTEXES") {
        if (polygons.empty()) {
            os << "0" << std::endl;
        }
        else {
            auto compareVertexes = [](const Polygon& a, const Polygon& b) {
                return a.points.size() < b.points.size();
                };
            if (isMax) {
                auto it = std::max_element(polygons.begin(), polygons.end(), compareVertexes);
                os << it->points.size() << std::endl;
            }
            else {
                auto it = std::min_element(polygons.begin(), polygons.end(), compareVertexes);
                os << it->points.size() << std::endl;
            }
        }
    }
    else {
        throw InvalidCommandException();
    }
}


void handleCount(std::istringstream& iss, const std::vector<Polygon>& polygons, std::ostream& os = std::cout)
{
    std::string arg;
    if (!(iss >> arg))
    {
        throw InvalidCommandException();
        return;
    }
    if (arg == "EVEN" || arg == "ODD")
    {
        if (!hasNoMoreArguments(iss))
        {
            throw InvalidCommandException();
            return;
        }
        bool wantEven = (arg == "EVEN");
        int count = std::count_if(polygons.begin(), polygons.end(),
            [wantEven](const Polygon& p) {
                return (p.points.size() % 2 == 0) == wantEven;
            });
        os << count << std::endl;
    }
    else
    {
        bool isNum = std::all_of(arg.begin(), arg.end(), ::isdigit);
        if (!isNum || !hasNoMoreArguments(iss))
        {
            throw InvalidCommandException();
            return;
        }
        int num = std::stoi(arg);
        int count = std::count_if(polygons.begin(), polygons.end(),
            [num](const Polygon& p) { return p.points.size() == num; });
        os << count << std::endl;
    }
}

void handleRects(const std::vector<Polygon>& polygons, std::ostream& os = std::cout)
{
    int count = std::count_if(polygons.begin(), polygons.end(), isRectangle);
    os << count << std::endl;
}

void handleSame(std::istringstream& iss, const std::vector<Polygon>& polygons, std::ostream& os = std::cout)
{
    int n;
    if (!(iss >> n) || n < 1)
    {
        throw InvalidCommandException();
        return;
    }
    Polygon target;
    for (int i = 0; i < n; ++i)
    {
        char c;
        int x, y;
        if (!(iss >> c) || c != '(' || !(iss >> x) ||
            !(iss >> c) || c != ';' || !(iss >> y) ||
            !(iss >> c) || c != ')') {
            throw InvalidCommandException();
            return;
        }
        target.points.emplace_back(Point{ x, y });
    }
    if (!hasNoMoreArguments(iss))
    {
        throw InvalidCommandException();
        return;
    }

    int count = std::count_if(polygons.begin(), polygons.end(),
        [&](const Polygon& p)
        {
            if (p.points.size() != target.points.size())
                return false;
            int dx = target.points[0].x - p.points[0].x;
            int dy = target.points[0].y - p.points[0].y;
            return std::equal(
                p.points.begin() + 1, p.points.end(),
                target.points.begin() + 1,
                [&](const Point& a, const Point& b) {
                    return a.x + dx == b.x && a.y + dy == b.y;
                }
            );
        }
    );
    os << count << std::endl;
}

#endif // !FUNCTION_H
