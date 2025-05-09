#include <string>
#include <iomanip>
#include <numeric>
#include <iterator>
#include <algorithm>

#include "IOFmtguard.h"
#include "Commands.h"
#include "DelimiterIO.h"

void cmd::area(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out)
{
    if (in.peek() == '\n')
    {
        throw std::invalid_argument("No param");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    std::string param = "";

    in >> DelimiterIO{ ' ' } >> param;

    if (in.peek() != '\n')
    {
        throw std::invalid_argument("Wrong line");
    }

    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "EVEN")
    {
        out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, subcmd::getAreaOfEven);
    }
    else if (param == "ODD")
    {
        out << std::accumulate(shapes.cbegin(), shapes.cend(), 0.0, subcmd::getAreaOfOdd);
    }
    else if (param == "MEAN")
    {
        if (shapes.size() > 0)
        {
            out << std::accumulate
            (
                shapes.cbegin(), shapes.cend(), 0.0, subcmd::getSumArea
            ) / shapes.size();
        }
        else
        {
            throw std::invalid_argument("No args for Mean");
        }
    }
    else
    {
        if (!std::all_of(param.begin(), param.end(), subcmd::isDigitButBool))
        {
            throw std::invalid_argument("Invalid param");
        }

        int vertexes = std::stoi(param);

        if (vertexes >= 3)
        {
            out << std::accumulate
            (
                shapes.cbegin(), shapes.cend(), 0.0,
                std::bind
                (
                    subcmd::getVertexesArea, std::placeholders::_1, std::placeholders::_2, vertexes
                )
            );
        }
        else
        {
            throw std::invalid_argument("Amount of vertexes must be more than 2");
        }
    }
}

void cmd::max(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out)
{
    if (shapes.size() == 0)
    {
        throw std::invalid_argument("No polygons");
    }

    if (in.peek() == '\n')
    {
        throw std::invalid_argument("No param");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    std::string param = "";

    in >> DelimiterIO{ ' ' } >> param;

    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);

    if (in.peek() != '\n')
    {
        throw std::invalid_argument("Wrong line");
    }

    if (param == "AREA")
    {
        out << subcmd::getPolygonArea(*std::max_element(
            shapes.cbegin(), shapes.cend(), subcmd::comparatorForArea
        ));
    }
    else if (param == "VERTEXES")
    {
        out << (*std::max_element
        (
            shapes.cbegin(), shapes.cend(), subcmd::comparatorForVertexes
        )).points.size();
    }
    else
    {
        throw std::invalid_argument("Invalid param");
    }
}

void cmd::min(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out)
{
    if (shapes.size() == 0)
    {
        throw std::invalid_argument("No polygons");
    }

    if (in.peek() == '\n')
    {
        throw std::invalid_argument("No param");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    std::string param = "";

    in >> DelimiterIO{ ' ' } >> param;

    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);

    if (in.peek() != '\n')
    {
        throw std::invalid_argument("Wrong line");
    }

    if (param == "AREA")
    {
        out << subcmd::getPolygonArea(*std::min_element
        (
            shapes.cbegin(), shapes.cend(), subcmd::comparatorForArea
        ));
    }
    else if (param == "VERTEXES")
    {
        out << (*std::min_element
        (
            shapes.cbegin(), shapes.cend(), subcmd::comparatorForVertexes
        )).points.size();
    }
    else
    {
        throw std::invalid_argument("Invalid param");
    }
}

void cmd::count(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out)
{
    if (in.peek() == '\n')
    {
        throw std::invalid_argument("No param");
    }

    iofmtguard ifmtguard(in);
    in >> std::noskipws;

    std::string param = "";


    in >> DelimiterIO{ ' ' } >> param;

    if (in.peek() != '\n')
    {
        throw std::invalid_argument("Wrong line");
    }

    iofmtguard ofmtguard(out);
    out << std::fixed << std::setprecision(1);

    if (param == "EVEN")
    {
        out << std::count_if(shapes.begin(), shapes.end(), subcmd::isEven);
    }
    else if (param == "ODD")
    {
        out << std::count_if(shapes.begin(), shapes.end(), subcmd::isOdd);
    }
    else
    {
        if (!std::all_of(param.begin(), param.end(), subcmd::isDigitButBool))
        {
            throw std::invalid_argument("Invalid param");
        }

        int vertexes = std::stoi(param);

        if (vertexes >= 3)
        {
            out << std::count_if
            (
                shapes.begin(), shapes.end(), std::bind
                (
                    subcmd::consistsFromGivenAmountOfVertexes, std::placeholders::_1, vertexes
                )
            );
        }
        else
        {
            throw std::invalid_argument("Amount of vertexes must be more than 2");
        }
    }
}

void cmd::inframe(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out)
{
    std::string answer = "<FALSE>";
    if (in.peek() == '\n')
    {
        throw std::invalid_argument("No polygon");
    }
    shapes::Polygon polygon;

    iofmtguard ifmtguard(in);
    in >> std::noskipws;
    in >> DelimiterIO{ ' ' } >> polygon;

    if (in.fail() && !in.eof())
    {
        in.clear();
        throw std::invalid_argument("Invalid polygon");
    }

    std::vector<int> minXVector(shapes.size());
    std::vector<int> maxXVector(shapes.size());
    std::vector<int> minYVector(shapes.size());
    std::vector<int> maxYVector(shapes.size());

    std::transform
    (
        shapes.begin(), shapes.end(),
        minXVector.begin(), subcmd::getMinX
    );
    std::transform
    (
        shapes.begin(), shapes.end(),
        maxXVector.begin(), subcmd::getMaxX
    );
    std::transform
    (
        shapes.begin(), shapes.end(),
        minYVector.begin(), subcmd::getMinY
    );
    std::transform
    (
        shapes.begin(), shapes.end(),
        maxYVector.begin(), subcmd::getMaxY
    );

    int minX = *std::min_element(minXVector.begin(), minXVector.end());
    int maxX = *std::max_element(maxXVector.begin(), maxXVector.end());
    int minY = *std::min_element(minYVector.begin(), minYVector.end());
    int maxY = *std::max_element(maxYVector.begin(), maxYVector.end());

    int minXOfPolygon = subcmd::getMinX(polygon);
    int maxXOfPolygon = subcmd::getMaxX(polygon);
    int minYOfPolygon = subcmd::getMinY(polygon);
    int maxYOfPolygon = subcmd::getMaxY(polygon);

    if (minXOfPolygon >= minX && minYOfPolygon >= minY && maxXOfPolygon <= maxX && maxYOfPolygon <= maxY)
    {
        answer = "<TRUE>";
    }
    out << answer;
}

void cmd::rightshapes(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out)
{
    if (in.peek() != '\n')
    {
        throw std::invalid_argument("No required param");
    }

    out << count_if(shapes.begin(), shapes.end(), subcmd::hasRightAngle);
}
