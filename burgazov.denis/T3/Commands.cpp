#include <string>
#include <iomanip>
#include <numeric>
#include <limits>
#include <iterator>
#include <algorithm>
#include <functional>

#include "Fmtguard.h"
#include "Commands.h"

void area(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{
    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    if (!(in >> DelimiterIO{ ' ' }))
    {
        throw std::invalid_argument("ERROR: Missing parameters");
    }

    std::string param = "";
    in >> param;

    if (in.peek() != '\n')
    {
        throw std::invalid_argument("ERROR: Wrong line");
    }

    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "EVEN")
    {
        out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, getAreaOfEven);
    }
    else if (param == "ODD")
    {
        out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, getAreaOfOdd);
    }
    else if (param == "MEAN")
    {
        if (shapes.size() > 0)
        {
            out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, getSumArea)
                / shapes.size();
        }
        else
        {
            throw std::invalid_argument("ERROR: No enough shapes");
        }
    }
    else
    {
        size_t vertexes = 0;
        try {
            vertexes = std::stoi(param);
        }
        catch (...) {
            throw std::invalid_argument("ERROR: Invalid parameters");
        }

        if (vertexes < 3) {
            throw std::invalid_argument("ERROR: Invalid parameters");
        }

        out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0,
            std::bind(getVertexesArea, std::placeholders::_1, std::placeholders::_2, vertexes));
    }
}

void max(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{
    if (shapes.size() == 0)
    {
        throw std::invalid_argument("ERROR: No enough shapes");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    if (!(in >> DelimiterIO{ ' ' }))
    {
        throw std::invalid_argument("ERROR: Missing parameters");
    }

    std::string param = "";
    in >> param;

    if (in.peek() != '\n')
    {
        throw std::invalid_argument("ERROR: Wrong line");
    }

    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "AREA")
    {
        out << std::accumulate(shapes.cbegin(), shapes.cend(), getPolygonArea(shapes[0]), getMaxArea);
    }
    else if (param == "VERTEXES")
    {
        out << std::accumulate(shapes.cbegin(), shapes.cend(), shapes[0].points.size(), getMaxVertexes);
    }
    else
    {
        throw std::invalid_argument("ERROR: Invalid parameters");
    }
}

void min(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{
    if (shapes.size() == 0)
    {
        throw std::invalid_argument("ERROR: No enough shapes");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    if (!(in >> DelimiterIO{ ' ' }))
    {
        throw std::invalid_argument("ERROR: Missing parameters");
    }

    std::string param = "";
    in >> param;

    if (in.peek() != '\n')
    {
        throw std::invalid_argument("ERROR: Wrong line");
    }

    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "AREA")
    {
        out << std::accumulate(shapes.cbegin(), shapes.cend(), getPolygonArea(shapes[0]), getMinArea);
    }
    else if (param == "VERTEXES")
    {
        out << std::accumulate(shapes.cbegin(), shapes.cend(), shapes[0].points.size(), getMinVertexes);
    }
    else
    {
        throw std::invalid_argument("ERROR: Invalid parameters");
    }
}

void count(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{
    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    if (!(in >> DelimiterIO{ ' ' }))
    {
        throw std::invalid_argument("ERROR: Missing parameters");
    }

    std::string param = "";
    in >> param;

    if (in.peek() != '\n')
    {
        throw std::invalid_argument("Wrong line");
    }

    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "EVEN")
    {
        out << std::count_if(shapes.begin(), shapes.end(), isEven);
    }
    else if (param == "ODD")
    {
        out << std::count_if(shapes.begin(), shapes.end(), isOdd);
    }
    else
    {
        int vertexes = 0;
        try {
            vertexes = std::stoi(param);
        }
        catch (...) {
            throw std::invalid_argument("ERROR: Invalid parameters");
        }

        if (vertexes < 3) {
            throw std::invalid_argument("ERROR: Invalid parameters");
        }

        out << std::count_if(shapes.begin(), shapes.end(),
            std::bind(isVertexes, std::placeholders::_1, vertexes));
    }
}

void inframe(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out)
{
    std::string answer = "<FALSE>";

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    Polygon polygon;
    in >> DelimiterIO{ ' ' } >> polygon;

    if (in.fail() && !in.eof())
    {
        in.clear();
        throw std::invalid_argument("ERROR: Invalid shape");
    }

    std::vector<int> minXVector(shapes.size());
    std::vector<int> maxXVector(shapes.size());
    std::vector<int> minYVector(shapes.size());
    std::vector<int> maxYVector(shapes.size());


    int minX = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int minY = std::numeric_limits<int>::max();
    int maxY = std::numeric_limits<int>::min();
    std::for_each(shapes.begin(), shapes.end(),
        std::bind(getFrame, std::ref(minX), std::ref(maxX), std::ref(minY), std::ref(maxY), std::placeholders::_1));

    int minXOfPolygon = std::numeric_limits<int>::max();
    int maxXOfPolygon = std::numeric_limits<int>::min();
    int minYOfPolygon = std::numeric_limits<int>::max();
    int maxYOfPolygon = std::numeric_limits<int>::min();
    getFrame(minXOfPolygon, maxXOfPolygon, minYOfPolygon, maxYOfPolygon, polygon);

    if (minXOfPolygon >= minX && minYOfPolygon >= minY && maxXOfPolygon <= maxX && maxYOfPolygon <= maxY)
    {
        answer = "<TRUE>";
    }
    out << answer;
}

void echo(std::vector< Polygon >& shapes, std::istream& in, std::ostream& out) {
    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    Polygon polygon;
    in >> DelimiterIO{ ' ' } >> polygon;

    if (in.fail() && !in.eof())
    {
        in.clear();
        throw std::invalid_argument("ERROR: Invalid shape");
    }

    std::vector < Polygon > newShapes;
    int countOfDublicate = 0;
    std::transform(shapes.begin(), shapes.end(), std::back_inserter(newShapes),
        std::bind(subEcho, std::placeholders::_1, std::cref(polygon), std::ref(newShapes), std::ref(countOfDublicate)));
    shapes = newShapes;

    out << countOfDublicate;
}
