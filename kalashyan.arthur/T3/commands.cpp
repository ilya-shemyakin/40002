#include "commands.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <numeric>
#include <algorithm>

static bool isEven(std::size_t n) {
    return n % 2 == 0;
}

static bool isValidSize(std::size_t n) {
    return n >= 3;
}

static bool parseSizeArgument(const std::string &arg, std::size_t &out) {
    try {
        out = std::stoul(arg);
    } catch (...) {
        return false;
    }
    return isValidSize(out);
}

static bool shouldIncludeForArea(const Polygon &p, const std::string &arg, std::size_t size) {
    if (arg == "EVEN") return isEven(p.points.size());
    if (arg == "ODD") return !isEven(p.points.size());
    if (arg == "MEAN") return true;
    return p.points.size() == size;
}

static void printMeanArea(const std::vector<Polygon> &polygons) {
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                                   [](double acc, const Polygon &p) {
                                       return acc + calculateArea(p);
                                   });

    total /= polygons.size();
    std::cout << std::fixed << std::setprecision(1) << total << "\n";
}

static void handleArea(const std::string &arg, const std::vector<Polygon> &polygons) {
    if (arg == "MEAN") {
        printMeanArea(polygons);
        return;
    }

    std::size_t v = 0;
    if (arg != "EVEN" && arg != "ODD" && !parseSizeArgument(arg, v)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    double total = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                                   [&](double acc, const Polygon &p) {
                                       return shouldIncludeForArea(p, arg, v) ? acc + calculateArea(p) : acc;
                                   });

    std::cout << std::fixed << std::setprecision(1) << total << "\n";
}


static void handleMaxMinArea(bool isMax, const std::vector<Polygon> &polygons) {
    auto comp = [](const Polygon &a, const Polygon &b) {
        return calculateArea(a) < calculateArea(b);
    };
    auto it = isMax
                  ? std::max_element(polygons.begin(), polygons.end(), comp)
                  : std::min_element(polygons.begin(), polygons.end(), comp);
    double value = it == polygons.end() ? 0.0 : calculateArea(*it);
    std::cout << std::fixed << std::setprecision(1) << value << "\n";
}

static void handleMaxMinSize(bool isMax, const std::vector<Polygon> &polygons) {
    auto comp = [](const Polygon &a, const Polygon &b) {
        return a.points.size() < b.points.size();
    };
    auto it = isMax
                  ? std::max_element(polygons.begin(), polygons.end(), comp)
                  : std::min_element(polygons.begin(), polygons.end(), comp);
    int value = it == polygons.end() ? 0 : static_cast<int>(it->points.size());
    std::cout << value << "\n";
}

static void handleMaxMin(const std::string &cmd, const std::string &arg, const std::vector<Polygon> &polygons) {
    if (polygons.empty()) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    bool isMax = (cmd == "MAX");
    if (arg == "AREA") handleMaxMinArea(isMax, polygons);
    else handleMaxMinSize(isMax, polygons);
}


static bool shouldIncludeForCount(const Polygon &p, const std::string &arg, std::size_t size) {
    if (arg == "EVEN") return isEven(p.points.size());
    if (arg == "ODD") return !isEven(p.points.size());
    return p.points.size() == size;
}

static void handleCount(const std::string &arg, const std::vector<Polygon> &polygons) {
    std::size_t v = 0;
    if (arg != "EVEN" && arg != "ODD" && !parseSizeArgument(arg, v)) {
        std::cout << "<INVALID COMMAND>\n";
        return;
    }

    int count = static_cast<int>(std::count_if(polygons.begin(), polygons.end(),
                                               [&](const Polygon &p) {
                                                   return shouldIncludeForCount(p, arg, v);
                                               }));

    std::cout << count << "\n";
}


static bool isSamePolygonSorted(const Polygon &p, const std::vector<Point> &sortedQuery) {
    if (p.points.size() != sortedQuery.size()) return false;
    auto sorted = p.points;
    std::sort(sorted.begin(), sorted.end());
    return sorted == sortedQuery;
}

static void handlePerms(const std::vector<Point> &sortedQuery, int expectedSize, const std::vector<Polygon> &polygons) {
    int count = static_cast<int>(std::count_if(polygons.begin(), polygons.end(),
                                               [&](const Polygon &p) {
                                                   return static_cast<int>(p.points.size()) == expectedSize &&
                                                          isSamePolygonSorted(p, sortedQuery);
                                               }));
    std::cout << count << "\n";
}

static void handleMaxSeq(const std::vector<Point> &sortedQuery, int expectedSize,
                         const std::vector<Polygon> &polygons) {
    struct State {
        int current = 0;
        int best = 0;
    };

    State result = std::accumulate(polygons.begin(), polygons.end(), State{},
                                   [&](State s, const Polygon &p) {
                                       if (static_cast<int>(p.points.size()) != expectedSize || !isSamePolygonSorted(
                                               p, sortedQuery)) {
                                           s.current = 0;
                                           return s;
                                       }
                                       s.current++;
                                       s.best = std::max(s.best, s.current);
                                       return s;
                                   });

    std::cout << result.best << "\n";
}

static void handlePermsOrMaxseq(const std::string &cmd, const std::string &rest, const std::vector<Polygon> &polygons) {
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

    if (cmd == "PERMS") handlePerms(sortedQuery, n, polygons);
    else handleMaxSeq(sortedQuery, n, polygons);
}

void processCommand(const std::string &input, const std::vector<Polygon> &polygons) {
    std::istringstream iss(input);
    std::string cmd;
    iss >> cmd;

    if (cmd == "AREA" || cmd == "MAX" || cmd == "MIN" || cmd == "COUNT") {
        std::string arg;
        iss >> arg;
        if (arg.empty()) {
            std::cout << "<INVALID COMMAND>\n";
            return;
        }

        if (cmd == "AREA") handleArea(arg, polygons);
        else if (cmd == "COUNT") handleCount(arg, polygons);
        else handleMaxMin(cmd, arg, polygons);
    } else if (cmd == "PERMS" || cmd == "MAXSEQ") {
        std::string rest;
        std::getline(iss, rest);
        handlePermsOrMaxseq(cmd, rest, polygons);
    } else {
        std::cout << "<INVALID COMMAND>\n";
    }
}
