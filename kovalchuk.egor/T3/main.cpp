#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include "function.h"

void processCommands(const std::vector<Polygon>& shapes) {
    std::string line;
    if (!std::getline(std::cin, line)) return;

    std::istringstream iss(line);
    std::string cmd;
    iss >> cmd;

    if (cmd == "AREA") {
        std::string arg;
        if (!(iss >> arg) || (iss >> std::ws, !iss.eof())) {
            std::cout << "<INVALID COMMAND>\n";
        }
        else if (arg == "EVEN" || arg == "ODD") {
            bool wantEven = (arg == "EVEN");
            double sum = std::accumulate(shapes.begin(), shapes.end(), 0.0,
                [&](double acc, const Polygon& p) {
                    return acc + ((p.points.size() % 2 == 0) == wantEven
                        ? polygonArea(p) : 0.0);
                });
            std::cout << std::fixed << std::setprecision(1) << sum << "\n";
        }
        else if (arg == "MEAN") {
            if (shapes.empty()) {
                std::cout << "<INVALID COMMAND>\n";
            }
            else {
                double total = std::accumulate(shapes.begin(), shapes.end(), 0.0,
                    [](double acc, const Polygon& p) {
                        return acc + polygonArea(p);
                    });
                std::cout << std::fixed << std::setprecision(1)
                    << (total / shapes.size()) << "\n";
            }
        }
        else {
            try {
                int n = std::stoi(arg);
                if (n < 3) {
                    std::cout << "<INVALID COMMAND>\n";
                }
                else {
                    double sum = std::accumulate(shapes.begin(), shapes.end(), 0.0,
                        [n](double acc, const Polygon& p) {
                            return acc + ((static_cast<int>(p.points.size()) == n)
                                ? polygonArea(p) : 0.0);
                        });
                    std::cout << std::fixed << std::setprecision(1) << sum << "\n";
                }
            }
            catch (...) {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
    }
    else if (cmd == "MAX" || cmd == "MIN") {
        bool isMax = (cmd == "MAX");
        std::string arg;
        if (!(iss >> arg) || (iss >> std::ws, !iss.eof())) {
            std::cout << "<INVALID COMMAND>\n";
        }
        else if (arg == "AREA") {
            if (shapes.empty()) {
                std::cout << "<INVALID COMMAND>\n";
            }
            else {
                double init = polygonArea(shapes.front());
                double res = std::accumulate(shapes.begin() + 1, shapes.end(), init,
                    [isMax](double acc, const Polygon& p) {
                        double a = polygonArea(p);
                        return isMax ? std::max(acc, a) : std::min(acc, a);
                    });
                std::cout << std::fixed << std::setprecision(1) << res << "\n";
            }
        }
        else if (arg == "VERTEXES") {
            if (shapes.empty()) {
                std::cout << "<INVALID COMMAND>\n";
            }
            else {
                int init = static_cast<int>(shapes.front().points.size());
                int res = std::accumulate(shapes.begin() + 1, shapes.end(), init,
                    [isMax](int acc, const Polygon& p) {
                        int v = static_cast<int>(p.points.size());
                        return isMax ? std::max(acc, v) : std::min(acc, v);
                    });
                std::cout << res << "\n";
            }
        }
        else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
    else if (cmd == "COUNT") {
        std::string arg;
        if (!(iss >> arg) || (iss >> std::ws, !iss.eof())) {
            std::cout << "<INVALID COMMAND>\n";
        }
        else if (arg == "EVEN" || arg == "ODD") {
            bool wantEven = (arg == "EVEN");
            int cnt = std::count_if(shapes.begin(), shapes.end(),
                [wantEven](const Polygon& p) {
                    return (p.points.size() % 2 == 0) == wantEven;
                });
            std::cout << cnt << "\n";
        }
        else {
            try {
                int n = std::stoi(arg);
                if (n < 3) {
                    std::cout << "<INVALID COMMAND>\n";
                }
                else {
                    int cnt = std::count_if(shapes.begin(), shapes.end(),
                        [n](const Polygon& p) {
                            return static_cast<int>(p.points.size()) == n;
                        });
                    std::cout << cnt << "\n";
                }
            }
            catch (...) {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
    }
    else if (cmd == "RECTS") {
        if (iss >> std::ws, !iss.eof()) {
            std::cout << "<INVALID COMMAND>\n";
        }
        else {
            int cnt = std::count_if(shapes.begin(), shapes.end(),
                [](const Polygon& p) { return isRectangle(p); });
            std::cout << cnt << "\n";
        }
    }
    else if (cmd == "MAXSEQ") {
        std::string rest = line.substr(6);
        if (rest.find(")(") != std::string::npos) {
            std::cout << "<INVALID COMMAND>\n";
        }
        else {
            std::istringstream issPoly(rest);
            Polygon target;
            if (!parsePolygon(issPoly, target) || (issPoly >> std::ws, !issPoly.eof())) {
                std::cout << "<INVALID COMMAND>\n";
            }
            else {
                auto pr = std::accumulate(shapes.begin(), shapes.end(),
                    std::make_pair(0, 0),
                    [target](std::pair<int, int> acc, const Polygon& p) {
                        if (p == target) {
                            int curr = acc.first + 1;
                            int mx = std::max(acc.second, curr);
                            return std::make_pair(curr, mx);
                        }
                        return std::make_pair(0, acc.second);
                    });
                std::cout << pr.second << "\n";
            }
        }
    }
    else {
        std::cout << "<INVALID COMMAND>\n";
    }

    processCommands(shapes);
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "RU");
    if (argc < 2) {
        std::cerr << "Fail: did not get the name of the file\n";
        return 1;
    }
    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Fail: could not open the file\n";
        return 1;
    }

    std::vector<Polygon> shapes;
    readPolygons(file, shapes);

    processCommands(shapes);
    return 0;
}
