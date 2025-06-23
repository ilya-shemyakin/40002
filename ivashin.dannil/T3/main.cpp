#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

struct Polygon {
    std::vector<Point> points;
    bool operator==(const Polygon& other) const {
        return points == other.points;
    }
};

bool doIntersect(const Polygon& p1, const Polygon& p2) {
    auto onSegment = [](const Point& p, const Point& q, const Point& r) {
        return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
        };
    auto orientation = [](const Point& p, const Point& q, const Point& r) {
        long long val = (static_cast<long long>(q.y) - p.y) * (r.x - q.x) -
            (static_cast<long long>(q.x) - p.x) * (r.y - q.y);
        return (val == 0) ? 0 : (val > 0) ? 1 : 2;
        };
    auto segmentsIntersect = [&](const Point& p1, const Point& q1,
        const Point& p2, const Point& q2) {
            int o1 = orientation(p1, q1, p2), o2 = orientation(p1, q1, q2);
            int o3 = orientation(p2, q2, p1), o4 = orientation(p2, q2, q1);
            if (o1 != o2 && o3 != o4) return true;
            if (o1 == 0 && onSegment(p1, p2, q1)) return true;
            if (o2 == 0 && onSegment(p1, q2, q1)) return true;
            if (o3 == 0 && onSegment(p2, p1, q2)) return true;
            if (o4 == 0 && onSegment(p2, q1, q2)) return true;
            return false;
        };
    if (p1 == p2) return true;
    for (size_t i = 0; i < p1.points.size(); ++i) {
        for (size_t j = 0; j < p2.points.size(); ++j) {
            if (segmentsIntersect(p1.points[i],
                p1.points[(i + 1) % p1.points.size()],
                p2.points[j],
                p2.points[(j + 1) % p2.points.size()])) {
                return true;
            }
        }
    }
    auto isPointInside = [&](const Point& pt, const Polygon& poly) {
        int crossings = 0;
        for (size_t i = 0; i < poly.points.size(); ++i) {
            Point a = poly.points[i];
            Point b = poly.points[(i + 1) % poly.points.size()];
            if ((a.y <= pt.y && pt.y < b.y) || (b.y <= pt.y && pt.y < a.y)) {
                if (pt.x < (b.x - a.x) * (pt.y - a.y) / (b.y - a.y) + a.x) {
                    crossings++;
                }
            }
        }
        return (crossings % 2) == 1;
        };
    for (const auto& pt : p1.points) {
        if (isPointInside(pt, p2)) return true;
    }
    for (const auto& pt : p2.points) {
        if (isPointInside(pt, p1)) return true;
    }
    return false;
}

Polygon parsePolygon(const std::string& line) {
    std::istringstream iss(line);
    int n;
    Polygon poly;
    if (!(iss >> n) || n < 3) return poly;
    for (int i = 0; i < n; ++i) {
        char c;
        int x, y;
        if (!(iss >> c) || c != '(') return Polygon();
        if (!(iss >> x >> c) || c != ';') return Polygon();
        if (!(iss >> y >> c) || c != ')') return Polygon();
        poly.points.push_back({ x, y });
    }
    std::string rest;
    if (std::getline(iss, rest) &&
        !rest.empty() && rest.find_first_not_of(" \t") != std::string::npos) {
        return Polygon();
    }
    return poly.points.size() == static_cast<size_t>(n) ? poly : Polygon();
}

std::vector<Polygon> readPolygons(const std::string& filename) {
    std::vector<Polygon> polygons;
    std::ifstream file(filename);
    if (!file.is_open()) return polygons;
    std::string line;
    while (std::getline(file, line)) {
        Polygon poly = parsePolygon(line);
        if (!poly.points.empty()) polygons.push_back(poly);
    }
    return polygons;
}

void processCommands(std::vector<Polygon>& polygons) {
    std::string command;
    while (std::cin >> command) {
        if (command == "RMECHO") {
            std::string line;
            std::getline(std::cin, line);
            Polygon target = parsePolygon(line);
            if (target.points.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            size_t oldSize = polygons.size();
            auto equalToTarget = std::bind(std::equal_to<Polygon>(),
                std::placeholders::_1, target);
            auto it = std::unique(polygons.begin(), polygons.end(),
                equalToTarget);
            polygons.erase(it, polygons.end());
            std::cout << oldSize - polygons.size() << '\n';
        }
        else if (command == "INTERSECTIONS") {
            std::string line;
            std::getline(std::cin, line);
            Polygon target = parsePolygon(line);
            if (target.points.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            auto intersectsWith = std::bind(doIntersect,
                std::placeholders::_1, target);
            size_t count = std::count_if(polygons.begin(), polygons.end(),
                intersectsWith);
            std::cout << count << '\n';
        }
        else {
            std::cout << "<INVALID COMMAND>\n";
            std::string line;
            std::getline(std::cin, line);
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: filename must be provided\n";
        return 1;
    }
    auto polygons = readPolygons(argv[1]);
    processCommands(polygons);
    return 0;
}