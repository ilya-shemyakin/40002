#include "commands.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <numeric>
#include <algorithm>

namespace {

void handleArea(const std::string& arg, const std::vector<Polygon>& polygons) {
    std::size_t v = 0;
    if (arg != "EVEN" && arg != "ODD" && arg != "MEAN") {
        try {
            v = std::stoul(arg);
        } catch (...) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        if (v < 3) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
    }

    double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
        [&](double acc, const Polygon& p) {
            bool take = (arg == "EVEN" || arg == "ODD")
                ? ((p.points.size() % 2 == 0) == (arg == "EVEN"))
                : (arg == "MEAN" ? true : p.points.size() == v);
            return take ? acc + calculateArea(p) : acc;
        });

    if (arg == "MEAN") {
        if (polygons.empty()) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        total /= polygons.size();
    }

    std::cout << std::fixed << std::setprecision(1) << total << "\n";
}

void handleMaxMin(const std::string& cmd, const std::string& arg, const std::vector<Polygon>& polygons) {
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    bool isMax = (cmd == "MAX");

    if (arg == "AREA") {
        auto comp = [](const Polygon& a, const Polygon& b) {
            return calculateArea(a) < calculateArea(b);
        };
        auto it = isMax
            ? std::max_element(polygons.begin(), polygons.end(), comp)
            : std::min_element(polygons.begin(), polygons.end(), comp);
        double value = it == polygons.end() ? 0.0 : calculateArea(*it);
        std::cout << std::fixed << std::setprecision(1) << value << "\n";
    } else {
        auto comp = [](Polygon const &a, Polygon const &b) {
            return a.points.size() < b.points.size();
        };
        auto it = isMax
            ? std::max_element(polygons.begin(), polygons.end(), comp)
            : std::min_element(polygons.begin(), polygons.end(), comp);
        int value = it == polygons.end() ? 0 : static_cast<int>(it->points.size());
        std::cout << value << "\n";
    }
}

void handleCount(const std::string& arg, const std::vector<Polygon>& polygons) {
    std::size_t v = 0;
    if (arg != "EVEN" && arg != "ODD") {
        try {
            v = std::stoul(arg);
        } catch (...) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        if (v < 3) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
    }

    int count = static_cast<int>(std::count_if(polygons.begin(), polygons.end(),
        [&](const Polygon& p) {
            return (arg == "EVEN" || arg == "ODD")
                ? ((p.points.size() % 2 == 0) == (arg == "EVEN"))
                : p.points.size() == v;
        }));
    std::cout << count << "\n";
}

void handlePermsOrMaxseq(const std::string& cmd, const std::string& rest, const std::vector<Polygon>& polygons) {
    if (polygons.empty() || rest.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    int n = static_cast<int>(std::count(rest.begin(), rest.end(), '('));
    std::string line = std::to_string(n) + " " + rest;

    Polygon query;
    if (!readPolygon(line, query)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    auto sortedQuery = query.points;
    std::sort(sortedQuery.begin(), sortedQuery.end());

    if (cmd == "PERMS") {
        int count = static_cast<int>(std::count_if(polygons.begin(), polygons.end(),
            [&](const Polygon& p) {
                if (static_cast<int>(p.points.size()) != n) return false;
                auto sorted = p.points;
                std::sort(sorted.begin(), sorted.end());
                return sorted == sortedQuery;
            }));
        std::cout << count << "\n";
    } else {
        struct State {
            int current = 0;
            int best = 0;
        };

        State result = std::accumulate(polygons.begin(), polygons.end(), State{},
            [&](State s, const Polygon& p) {
                if (static_cast<int>(p.points.size()) != n) {
                    s.current = 0;
                    return s;
                }

                auto sorted = p.points;
                std::sort(sorted.begin(), sorted.end());
                if (sorted == sortedQuery) {
                    s.current++;
                    s.best = std::max(s.best, s.current);
                } else {
                    s.current = 0;
                }
                return s;
            });

        std::cout << result.best << "\n";
    }
}

}

void processCommand(const std::string &input, const std::vector<Polygon> &polygons) {
    std::istringstream iss(input);
    std::string cmd;
    iss >> cmd;

    if (cmd == "AREA") {
        std::string arg;
        iss >> arg;
        if (arg.empty()) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        handleArea(arg, polygons);
    }
    else if (cmd == "MAX" || cmd == "MIN") {
        std::string arg;
        iss >> arg;
        if (arg.empty()) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        handleMaxMin(cmd, arg, polygons);
    }
    else if (cmd == "COUNT") {
        std::string arg;
        iss >> arg;
        if (arg.empty()) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }
        handleCount(arg, polygons);
    }
    else if (cmd == "PERMS" || cmd == "MAXSEQ") {
        std::string rest;
        std::getline(iss, rest);
        handlePermsOrMaxseq(cmd, rest, polygons);
    }
    else {
        std::cout << "<INVALID COMMAND>\n";
    }
}