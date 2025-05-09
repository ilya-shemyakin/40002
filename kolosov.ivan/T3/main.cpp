#include "Commands.h"
#include "FillVectorOfShapes.h"
#include "IOFmtguard.h"

int main(int argc, char* argv[])
{
    std::string filename;
    if (argc != 2)
    {
        std::cout << "ERROR: expected filename as only command-line argument\n";
        return -1;
    }
        filename = argv[1];

    std::vector<shapes::Polygon> shapes;
    try
    {
        shapes = shapes::fillVectorOfShapes(filename);
    }
    catch (std::invalid_argument& ex)
    {
        std::cout << ex.what() << '\n';
        return -13;
    }

    std::map<std::string, std::function<void(std::istream& in, std::ostream& out)>> cmds;
    cmds["AREA"] = std::bind(cmd::area, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    cmds["MAX"] = std::bind(cmd::max, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    cmds["MIN"] = std::bind(cmd::min, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    cmds["COUNT"] = std::bind(cmd::count, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    cmds["INFRAME"] = std::bind(cmd::inframe, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);
    cmds["RIGHTSHAPES"] = std::bind(cmd::rightshapes, std::cref(shapes), std::placeholders::_1, std::placeholders::_2);

    iofmtguard ofmtguard(std::cout);
    std::cout << std::fixed << std::setprecision(1);
    std::string command = "";
    while (std::cin >> command)
    {
        try
        {
            cmds.at(command)(std::cin, std::cout);
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
