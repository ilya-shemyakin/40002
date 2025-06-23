#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "polygon.h"
#include "commands.h"

int main(int argc, char *argv[]) {
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
        if (!input.empty()) {
            processCommand(input, polygons);
        }
    }

    return 0;
}
