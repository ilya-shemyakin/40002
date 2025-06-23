//Variant 5. ECHO. INFRAME.
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <iterator>
#include <algorithm>
#include <map>
#include <functional>
#include <iomanip>

#include "Shapes.h"
#include "Fmtguard.h"
#include "Commands.h"

void readfile(std::string filename, std::vector<Polygon>& shapes) {
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::invalid_argument("ERROR: The file cannot be opened.");
    }

    while (!file.eof())
    {
        std::copy (
            std::istream_iterator< Polygon >(file),
            std::istream_iterator< Polygon >(),
            std::back_inserter(shapes)
        );
        if (file.fail() && !file.eof()) {
            file.clear();
            file.ignore(std::numeric_limits< std::streamsize>::max(), '\n');
        }
    }

    file.close();
}

int main(int argc, char* argv[])
{
    std::string filename;
    if (argc != 2)
    {
        std::cout << "ERROR: File name not specified\n";
        return -1;
    }
    filename = argv[1];

    std::vector<Polygon> shapes;
    try 
    {
        readfile(filename, shapes);
    }
    catch (std::exception& e) {
        std::cout << e.what() << '\n';
        return -1;
    }
    
    std::map<std::string, std::function<void(std::istream& in, std::ostream& out)>> commands;
    commands["AREA"] = std::bind(area, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    commands["MAX"] = std::bind(max, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    commands["MIN"] = std::bind(min, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    commands["COUNT"] = std::bind(count, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    commands["INFRAME"] = std::bind(inframe, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    commands["ECHO"] = std::bind(echo, std::ref(shapes), std::placeholders::_1, std::placeholders::_2);

    iofmtguard ofmtguard(std::cout);
    std::cout << std::fixed << std::setprecision(1);
    std::string command = "";
    while (std::cin >> command)
    {
        try
        {
            commands.at(command)(std::cin, std::cout);
            std::cout << '\n';
        }
        catch (...)
        {
            std::cout << "<INVALID COMMAND>\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
        }
    }

    return 0;
}
