#include  "function.h"
#include <iostream>

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
        try
        {
            if (!(iss >> cmd))
            {
                throw InvalidCommandException();
                continue;
            }
            if (cmd == "AREA") {
                handleArea(iss, polygons);
            }
            else if (cmd == "MAX") {
                handleExtremum(iss, polygons, std::cout, true);
            }
            else if (cmd == "MIN") {
                handleExtremum(iss, polygons, std::cout, false);
            }
            else if (cmd == "COUNT") {
                handleCount(iss, polygons);
            }
            else if (cmd == "RECTS") {
                if (hasNoMoreArguments(iss))
                    handleRects(polygons);
                else
                    std::cout << "<INVALID COMMAND>" << std::endl;
            }
            else if (cmd == "SAME") {
                handleSame(iss, polygons);
            }
            else {
                std::cout << "<INVALID COMMAND>" << std::endl;
            }
        }
        catch (const InvalidCommandException& e) {
            std::cout << e.what() << std::endl;
        }
    }
    return 0;
}
