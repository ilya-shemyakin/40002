#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <iomanip>

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


bool isRectangle(const Polygon& poly)
{
    if (poly.points.size() != 4)
        return false;
    std::vector<Point> vectors(4);
    for (int i = 0; i < 4; ++i)
    {
        vectors[i] = {
            poly.points[(i + 1) % 4].x - poly.points[i].x,
            poly.points[(i + 1) % 4].y - poly.points[i].y
        };
    }
    for (int i = 0; i < 4; ++i)
    {
        int next = (i + 1) % 4;
        long long dot = static_cast<long long>(vectors[i].x) * vectors[next].x +
            static_cast<long long>(vectors[i].y) * vectors[next].y;
        if (dot != 0)
            return false;
    }
    return true;
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
                !(iss >> c) || c != ')') {
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
    return iss.eof();
}

void handleArea(std::istringstream& iss, const std::vector<Polygon>& polygons)
{
    std::string arg;
    if (!(iss >> arg))
    {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (arg == "EVEN" || arg == "ODD")
    {
        if (!hasNoMoreArguments(iss))
        {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        bool wantEven = (arg == "EVEN");
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [wantEven](double acc, const Polygon& p)
            {
                bool isEven = (p.points.size() % 2 == 0);
                return acc + (isEven == wantEven ? polygonArea(p) : 0);
            });
        std::cout << sum << std::endl;
    }
    else if (arg == "MEAN") {
        if (!hasNoMoreArguments(iss))
        {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        if (polygons.empty())
            std::cout << "0.0" << std::endl;
        else
        {
            double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [](double acc, const Polygon& p) { return acc + polygonArea(p); });
            std::cout << (total / polygons.size()) << std::endl;
        }
    }
    else
    {
        bool isNum = std::all_of(arg.begin(), arg.end(), ::isdigit);
        if (!isNum || !hasNoMoreArguments(iss))
        {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        int num = std::stoi(arg);
        double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
            [num](double acc, const Polygon& p) {
                return acc + (p.points.size() == num ? polygonArea(p) : 0);
            });
        std::cout << sum << std::endl;
    }
}


template <typename Comparator>
void handleExtremum(std::istringstream& iss, const std::vector<Polygon>& polygons,
    Comparator comp, double init)
{
    std::string arg;
    if (!(iss >> arg) || !hasNoMoreArguments(iss))
    {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (arg == "AREA")
    {
        if (polygons.empty())
            std::cout << "0.0" << std::endl;
        else
        {
            double res = std::accumulate(polygons.begin(), polygons.end(), init,
                [comp](double acc, const Polygon& p) {
                    return comp(acc, polygonArea(p)) ? acc : polygonArea(p);
                });
            std::cout << res << std::endl;
        }
    }
    else if (arg == "VERTEXES") {
        if (polygons.empty()) std::cout << "0" << std::endl;
        else {
            int res = std::accumulate(polygons.begin(), polygons.end(),
                (init == 0 ? 0 : polygons[0].points.size()),
                [comp](int acc, const Polygon& p) {
                    return comp(acc, p.points.size()) ? acc : p.points.size();
                });
            std::cout << res << std::endl;
        }
    }
    else
    {
        std::cout << "<INVALID COMMAND>" << std::endl;
    }
}

void handleCount(std::istringstream& iss, const std::vector<Polygon>& polygons)
{
    std::string arg;
    if (!(iss >> arg))
    {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    if (arg == "EVEN" || arg == "ODD")
    {
        if (!hasNoMoreArguments(iss))
        {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        bool wantEven = (arg == "EVEN");
        int count = std::count_if(polygons.begin(), polygons.end(),
            [wantEven](const Polygon& p) {
                return (p.points.size() % 2 == 0) == wantEven;
            });
        std::cout << count << std::endl;
    }
    else
    {
        bool isNum = std::all_of(arg.begin(), arg.end(), ::isdigit);
        if (!isNum || !hasNoMoreArguments(iss))
        {
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        int num = std::stoi(arg);
        int count = std::count_if(polygons.begin(), polygons.end(),
            [num](const Polygon& p) { return p.points.size() == num; });
        std::cout << count << std::endl;
    }
}

void handleRects(const std::vector<Polygon>& polygons)
{
    int count = std::count_if(polygons.begin(), polygons.end(), isRectangle);
    std::cout << count << std::endl;
}

void handleSame(std::istringstream& iss, const std::vector<Polygon>& polygons)
{
    int n;
    if (!(iss >> n) || n < 1)
    {
        std::cout << "<INVALID COMMAND>" << std::endl;
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
            std::cout << "<INVALID COMMAND>" << std::endl;
            return;
        }
        target.points.emplace_back(Point{ x, y });
    }
    if (!hasNoMoreArguments(iss))
    {
        std::cout << "<INVALID COMMAND>" << std::endl;
        return;
    }
    int count = 0;
    for (const auto& p : polygons)
    {
        if (p.points.size() != target.points.size())
            continue;
        int dx = target.points[0].x - p.points[0].x;
        int dy = target.points[0].y - p.points[0].y;
        bool same = true;
        for (size_t i = 1; i < p.points.size(); ++i)
        {
            if (p.points[i].x + dx != target.points[i].x ||
                p.points[i].y + dy != target.points[i].y)
            {
                same = false;
                break;
            }
        }
        if (same)
            count++;
    }
    std::cout << count << std::endl;
}

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Error: filename not provided\n";
        return 1;
    }
    std::ifstream fin(argv[1]);
    if (!fin)
    {
        std::cerr << "Error: cannot open file\n";
        return 1;
    }
    std::vector<Polygon> polygons = readPolygons(fin);
    fin.close();

    std::string line;
    std::cout << std::fixed << std::setprecision(1);
    while (std::getline(std::cin, line))
    {
        if (line.empty())
            continue;
        std::istringstream iss(line);
        std::string cmd;
        if (!(iss >> cmd))
        {
            std::cout << "<INVALID COMMAND>" << std::endl;
            continue;
        }
        if (cmd == "AREA") handleArea(iss, polygons);
        else if (cmd == "MAX")
            handleExtremum(iss, polygons, [](double a, double b) { return a > b; }, 0.0);
        else if (cmd == "MIN")
            handleExtremum(iss, polygons, [](double a, double b) { return a < b; }, 1e20);
        else if (cmd == "COUNT")
            handleCount(iss, polygons);
        else if (cmd == "RECTS")
        {
            if (hasNoMoreArguments(iss))
                handleRects(polygons);
            else
                std::cout << "<INVALID COMMAND>" << std::endl;
        }
        else if (cmd == "SAME")
            handleSame(iss, polygons);
        else
            std::cout << "<INVALID COMMAND>" << std::endl;
    }
    return 0;
}
