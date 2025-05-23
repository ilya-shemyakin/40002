#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <functional>
#include <iomanip>
#include <limits>

struct Point {
    int x, y;
    bool operator==(Point const& o) const { return x==o.x && y==o.y; }
    bool operator< (Point const& o) const { return x<o.x || (x==o.x && y<o.y); }
};

struct Polygon {
    std::vector<Point> points;
};

bool readPolygon(const std::string& line, Polygon& poly) {
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
            if (!isspace(static_cast<unsigned char>(c))) {
                return false;
            }
        }
    }

    return poly.points.size() == static_cast<size_t>(n);
}

double calculateArea(Polygon const& p) {
    auto it0 = p.points.begin();
    auto it1 = std::next(it0);
    double cross0 = it0->x * p.points.back().y - it0->y * p.points.back().x;
    double sum = std::inner_product(
        it1, p.points.end(),
        it0, cross0,
        std::plus<>(),
        [](Point const& a, Point const& b){
            return a.x * b.y - a.y * b.x;
        }
    );
    return std::abs(sum) * 0.5;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Error: Filename required\n";
        return 1;
    }
    std::ifstream fin(argv[1]);
    if (!fin) {
        std::cerr << "Error: Cannot open file\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty()) continue;
        Polygon poly;
        if (readPolygon(line, poly)) {
            polygons.push_back(std::move(poly));
        }
    }

    std::string input;
    while (std::getline(std::cin, input)) {
        if (input.empty()) continue;
        std::istringstream iss(input);
        std::string cmd;
        iss >> cmd;

        if (cmd == "AREA") {
            std::string arg; iss >> arg;
            std::size_t v = 0;
            if (arg != "EVEN" && arg != "ODD" && arg != "MEAN") {
                v = std::stoul(arg);
                if (v < 3) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
            }

            double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                [&](double acc, auto const& p) {
                    bool take = (arg == "EVEN" || arg == "ODD")
                        ? ((p.points.size() % 2 == 0) == (arg == "EVEN"))
                        : (arg == "MEAN" ? true : p.points.size() == v);
                    return take ? acc + calculateArea(p) : acc;
                });

            if (polygons.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            if (arg == "MEAN") total /= polygons.size();

            std::cout << std::fixed << std::setprecision(1) << total << "\n";
        }

        else if (cmd == "MAX" || cmd == "MIN") {
            if (polygons.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            std::string arg; iss >> arg;
            bool isMax = (cmd == "MAX");

            if (arg == "AREA") {
                auto comp = [](Polygon const& a, Polygon const& b) {
                    return calculateArea(a) < calculateArea(b);
                };
                auto it = isMax
                    ? std::max_element(polygons.begin(), polygons.end(), comp)
                    : std::min_element(polygons.begin(), polygons.end(), comp);
                double v = it == polygons.end() ? 0.0 : calculateArea(*it);
                std::cout << std::fixed << std::setprecision(1) << v << "\n";
            } else {
                auto comp = [](Polygon const& a, Polygon const& b) {
                    return a.points.size() < b.points.size();
                };
                auto it = isMax
                    ? std::max_element(polygons.begin(), polygons.end(), comp)
                    : std::min_element(polygons.begin(), polygons.end(), comp);
                int v = it == polygons.end() ? 0 : static_cast<int>(it->points.size());
                std::cout << v << "\n";
            }
        }

        else if (cmd == "COUNT") {
            std::string arg; iss >> arg;
            std::size_t v = 0;
            if (arg != "EVEN" && arg != "ODD") {
                v = std::stoul(arg);
                if (v < 3) {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
            }

            int cnt = static_cast<int>(std::count_if(polygons.begin(), polygons.end(),
                [&](auto const& p) {
                    return (arg == "EVEN" || arg == "ODD")
                        ? ((p.points.size() % 2 == 0) == (arg == "EVEN"))
                        : p.points.size() == v;
                }));
            std::cout << cnt << "\n";
        }

        else if (cmd == "PERMS" || cmd == "MAXSEQ") {
            if (polygons.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            std::string rest;
            std::getline(iss, rest);
            if (rest.empty()) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            int n = static_cast<int>(std::count(rest.begin(), rest.end(), '('));
            std::string polyLine = std::to_string(n) + " " + rest;
            Polygon query;
            if (!readPolygon(polyLine, query)) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            auto normQ = query.points;
            std::sort(normQ.begin(), normQ.end());

            if (cmd == "PERMS") {
                int c = static_cast<int>(
                    std::count_if(polygons.begin(), polygons.end(),
                        [&](Polygon const& p) {
                            if (static_cast<int>(p.points.size()) != n) return false;
                            auto tmp = p.points;
                            std::sort(tmp.begin(), tmp.end());
                            return tmp == normQ;
                        }));
                std::cout << c << "\n";
            } else {
                struct S { int cur, best; };
                S r = std::accumulate(polygons.begin(), polygons.end(), S{ 0,0 },
                    [&](S s, Polygon const& p) {
                        if (static_cast<int>(p.points.size()) != n) {
                            s.cur = 0; return s;
                        }
                        auto tmp = p.points; std::sort(tmp.begin(), tmp.end());
                        if (tmp == normQ) {
                            s.cur++; s.best = std::max(s.best, s.cur);
                        } else s.cur = 0;
                        return s;
                    });
                std::cout << r.best << "\n";
            }
        }

        else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
    return 0;
}
