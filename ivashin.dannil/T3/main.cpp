#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <functional>
#include <numeric>
#include <cmath>
#include <limits>

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

double calculateArea(const Polygon& poly) {
    double area = 0.0;
    size_t n = poly.points.size();
    for (size_t i = 0; i < n; ++i) {
        size_t j = (i + 1) % n;
        area += poly.points[i].x * poly.points[j].y;
        area -= poly.points[j].x * poly.points[i].y;
    }
    return std::abs(area) / 2.0;
}

bool isSameShape(const Polygon& p1, const Polygon& p2) {
    if (p1.points.size() != p2.points.size()) return false;
    std::vector<Point> v1 = p1.points, v2 = p2.points;
    for (size_t i = 0; i < v1.size(); ++i) {
        if (std::equal(v1.begin(), v1.end(), v2.begin())) return true;
        std::rotate(v1.begin(), v1.begin() + 1, v1.end());
    }
    return false;
}

bool doIntersect(const Polygon& p1, const Polygon& p2) {
    auto onSegment = [](const Point& p, const Point& q, const Point& r) {
        return q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
            q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y);
        };
    auto orientation = [](const Point& p, const Point& q, const Point& r) {
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
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

bool isInFrame(const Polygon& poly, const std::vector<Polygon>& polygons) {
    if (poly.points.empty() || polygons.empty()) return false;
    int minX = std::numeric_limits<int>::max(), minY = minX;
    int maxX = std::numeric_limits<int>::min(), maxY = maxX;
    for (const auto& p : polygons) {
        for (const auto& point : p.points) {
            minX = std::min(minX, point.x);
            minY = std::min(minY, point.y);
            maxX = std::max(maxX, point.x);
            maxY = std::max(maxY, point.y);
        }
    }
    for (const auto& point : poly.points) {
        if (point.x < minX || point.x > maxX ||
            point.y < minY || point.y > maxY) {
            return false;
        }
    }
    return true;
}

void processCommands(std::vector<Polygon>& polygons) {
    std::cout << std::fixed << std::setprecision(1);
    std::string command;
    while (std::cin >> command) {
        if (command == "AREA") {
            std::string param;
            std::cin >> param;
            if (param == "EVEN" || param == "ODD") {
                bool even = param == "EVEN";
                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [even](double acc, const Polygon& p) {
                        return acc + (p.points.size() % 2 == (even ? 0 : 1) ?
                            calculateArea(p) : 0.0);
                    });
                std::cout << sum << '\n';
            }
            else if (param == "MEAN") {
                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [](double acc, const Polygon& p) {
                        return acc + calculateArea(p);
                    });
                std::cout << (polygons.empty() ? 0.0 : sum / polygons.size())
                    << '\n';
            }
            else {
                try {
                    int n = std::stoi(param);
                    if (n < 3) throw std::invalid_argument("Vertex count < 3");
                    double sum = std::accumulate(polygons.begin(), polygons.end(),
                        0.0, [n](double acc, const Polygon& p) {
                            return acc + (p.points.size() == static_cast<size_t>(n) ?
                                calculateArea(p) : 0.0);
                        });
                    std::cout << sum << '\n';
                }
                catch (...) {
                    std::cout << "<INVALID COMMAND>\n";
                }
            }
        }
        else if (command == "MAX") {
            std::string param;
            std::cin >> param;
            if (polygons.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (param == "AREA") {
                auto it = std::max_element(polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) {
                        return calculateArea(a) < calculateArea(b);
                    });
                std::cout << calculateArea(*it) << '\n';
            }
            else if (param == "VERTEXES") {
                auto it = std::max_element(polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) {
                        return a.points.size() < b.points.size();
                    });
                std::cout << it->points.size() << '\n';
            }
            else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else if (command == "MIN") {
            std::string param;
            std::cin >> param;
            if (polygons.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (param == "AREA") {
                auto it = std::min_element(polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) {
                        return calculateArea(a) < calculateArea(b);
                    });
                std::cout << calculateArea(*it) << '\n';
            }
            else if (param == "VERTEXES") {
                auto it = std::min_element(polygons.begin(), polygons.end(),
                    [](const Polygon& a, const Polygon& b) {
                        return a.points.size() < b.points.size();
                    });
                std::cout << it->points.size() << '\n';
            }
            else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else if (command == "COUNT") {
            std::string param;
            std::cin >> param;
            if (param == "EVEN" || param == "ODD") {
                bool even = param == "EVEN";
                size_t count = std::count_if(polygons.begin(), polygons.end(),
                    [even](const Polygon& p) {
                        return p.points.size() % 2 == (even ? 0 : 1);
                    });
                std::cout << count << '\n';
            }
            else {
                try {
                    int n = std::stoi(param);
                    if (n < 3) throw std::invalid_argument("Vertex count < 3");
                    size_t count = std::count_if(polygons.begin(), polygons.end(),
                        [n](const Polygon& p) {
                            return p.points.size() == static_cast<size_t>(n);
                        });
                    std::cout << count << '\n';
                }
                catch (...) {
                    std::cout << "<INVALID COMMAND>\n";
                }
            }
        }
        else if (command == "RMECHO") {
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
        else if (command == "LESSAREA") {
            std::string line;
            std::getline(std::cin, line);
            Polygon target = parsePolygon(line);
            if (target.points.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            double targetArea = calculateArea(target);
            size_t count = std::count_if(polygons.begin(), polygons.end(),
                [targetArea](const Polygon& p) {
                    return calculateArea(p) < targetArea;
                });
            std::cout << count << '\n';
        }
        else if (command == "INFRAME") {
            std::string line;
            std::getline(std::cin, line);
            Polygon target = parsePolygon(line);
            if (target.points.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            std::cout << (isInFrame(target, polygons) ? "<TRUE>" : "<FALSE>")
                << '\n';
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
