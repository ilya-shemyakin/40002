#include <iostream>
#include <fstream>
#include <string>
#include "command.h"
#include "geo.h"
#include <iomanip>
#include <sstream>
#include <limits>

int main(int argc, char* argv[]) {
    const int BAD_RESULT = -1;
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " filename\n";
        return BAD_RESULT;
    }

    std::ifstream file(argv[1]);
    if (!file) {
        std::cerr << "Failed to open file: " << argv[1] << "\n";
        return BAD_RESULT;
    }
    Polygon p;
    std::vector<Polygon> geoFigs;
    while (!file.eof())
    {
        std::copy(std::istream_iterator<Polygon>(file), std::istream_iterator<Polygon>(), std::back_inserter(geoFigs));
        if (!file) {
            file.clear();
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    file.close();
    std::ifstream fileCom("file.txt");
    //    std::cout << geoFigs.size();
        //std::istream& fileCom = std::cin;
    std::string command;
    double res;
    while (!fileCom.eof()) {
        fileCom >> command;
        res = BAD_RESULT;
        iofmtguard guard(std::cout);
        if (command == "AREA") {
            iofmtguard guard(std::cout);
            fileCom >> command;
            res = Area(geoFigs, command);
            if (res != BAD_RESULT) {
                // std::cout << "AREA (delete this message) " << command << " " << "\n";
                std::cout << std::fixed << std::setprecision(1) << res << "\n";
            }
            else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else if (command == "MAX") {
            fileCom >> command;
            res = maxSometh(geoFigs, command);
            if (res != BAD_RESULT) {
                //   std::cout << "MAX (delete this message) " << command << " " << "\n";
                std::cout << res << "\n";
            }
            else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else if (command == "MIN") {
            fileCom >> command;
            res = minSometh(geoFigs, command);
            if (res != BAD_RESULT) {
                //  std::cout << "MIN (delete this message) " << command << " " << "\n";
                std::cout << res << "\n";
            }
            else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else if (command == "COUNT") {
            fileCom >> command;
            res = countSomt(geoFigs, command);
            if (res != BAD_RESULT) {
                std::cout << res << "\n";
            }
            else {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else if (command == "INFRAME") {
            std::string line;
            std::getline(fileCom, line);
            std::istringstream iss(line);
            Polygon p;
            iss >> p;
            if (!iss) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            int flag = checkPolygonInFrame(p, geoFigs);
            if (flag == BAD_RESULT) {
                std::cout << "<INVALID COMMAND>\n";
            }
            else {
                //    std::cout << command << " ";
                if (flag) {
                    std::cout << "<TRUE>" << "\n";
                }
                else {
                    std::cout << "<FALSE>" << "\n";
                }
            }
        }
        else if (command == "PERMS") {
            std::string line;
            std::getline(fileCom, line);
            std::istringstream iss(line);
            Polygon p;
            iss >> p;
            if (!iss) {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            int flag = perms(p, geoFigs);
            if (flag == BAD_RESULT) {
                std::cout << "<INVALID COMMAND>\n";
            }
            else {
                //std::cout << command << " ";
                std::cout << perms(p, geoFigs) << "\n";
            }
        }
        else {
            std::cout << "<INVALID COMMAND>\n";
        }
    }
    return 0;
}

