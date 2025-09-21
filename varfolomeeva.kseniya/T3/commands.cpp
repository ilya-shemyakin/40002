#include "commands.hpp"
#include <functional>
#include <map>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include "streamGuard.hpp"
#include<string>

struct HasNumOfVertexes
{
    size_t vertexNum;
    explicit HasNumOfVertexes(size_t num);
    bool operator()(const varfolomeeva::Polygon& poly) const;
};

std::pair< varfolomeeva::Point, varfolomeeva::Point > makePair(const varfolomeeva::Point& p1, const varfolomeeva::Point& p2);
double shoelaceFormula(double acc, const std::pair< varfolomeeva::Point, varfolomeeva::Point >& points);
double sumArea(double acc, const varfolomeeva::Polygon& poly);
double calcArea(const varfolomeeva::Polygon& poly);
bool isEven(const varfolomeeva::Polygon& poly);
bool isOdd(const varfolomeeva::Polygon& poly);
double calcEvenArea(const std::vector< varfolomeeva::Polygon >& poly);
double calcOddArea(const std::vector< varfolomeeva::Polygon >& poly);
double calcMeanArea(const std::vector< varfolomeeva::Polygon >& poly);
double calcNumVertexArea(const std::vector< varfolomeeva::Polygon >& poly, size_t vertexNum);
bool compareVertexes(const varfolomeeva::Polygon& p1, const varfolomeeva::Polygon& p2);
double getMaxArea(const std::vector< varfolomeeva::Polygon >& poly);
size_t getMaxVertexes(const std::vector< varfolomeeva::Polygon >& poly);
double getMinArea(const std::vector< varfolomeeva::Polygon >& poly);
size_t getMinVertexes(const std::vector< varfolomeeva::Polygon >& poly);
size_t countEven(const std::vector< varfolomeeva::Polygon >& poly);
size_t countOdd(const std::vector< varfolomeeva::Polygon >& poly);
size_t countNum(const std::vector< varfolomeeva::Polygon >& poly, size_t vertexNum);
bool isIdentical(const varfolomeeva::Polygon& p1, const varfolomeeva::Polygon& p2, size_t& counter);
bool isSame(const varfolomeeva::Polygon& p1, const varfolomeeva::Polygon& p2);
bool isSamePoint(const varfolomeeva::Point& point, int x, int y, const varfolomeeva::Polygon& poly);

struct MaxSeqCounter
{
    size_t currentSeqLength;
    size_t maxSeqLength;
    bool operator()(const varfolomeeva::Polygon& poly, const varfolomeeva::Polygon& data);
};
std::pair< varfolomeeva::Point, varfolomeeva::Point > makePair(const varfolomeeva::Point& p1, const varfolomeeva::Point& p2)
{
    return std::make_pair(p1, p2);
}

double shoelaceFormula(double acc, const std::pair< varfolomeeva::Point, varfolomeeva::Point >& points)
{
    double area = points.first.x * points.second.y - points.second.x * points.first.y;
    return acc + area;
}

double sumArea(double acc, const varfolomeeva::Polygon& poly)
{
    return acc + calcArea(poly);
}
double calcArea(const varfolomeeva::Polygon& poly)
{
    std::vector< std::pair< varfolomeeva::Point, varfolomeeva::Point > > pointPairs;
    std::transform(poly.points.begin(), poly.points.end() - 1, poly.points.begin() + 1,
        std::back_inserter(pointPairs), makePair);
    pointPairs.emplace_back(poly.points.back(), poly.points.front());
    double areaSum = std::accumulate(pointPairs.begin(), pointPairs.end(), 0.0, shoelaceFormula);
    return 0.5 * std::abs(areaSum);
}

void varfolomeeva::doArea(std::vector< Polygon >& poly, std::istream& in, std::ostream& out)
{
    double result{};
    std::string subcommand;
    in >> subcommand;

    std::map< std::string, std::function< double() > > cmds;
    using namespace std::placeholders;
    cmds["EVEN"] = std::bind(calcEvenArea, poly);
    cmds["ODD"] = std::bind(calcOddArea, poly);
    cmds["MEAN"] = std::bind(calcMeanArea, poly);

    size_t vertexNum{};
    try
    {
        vertexNum = std::stoull(subcommand);
        if (vertexNum < 3)
        {
            throw std::logic_error("<INCORRECT VERTEX NUMBER>");
        }
        result = calcNumVertexArea(poly, vertexNum);
    }
    catch (const std::invalid_argument&)
    {
        if (cmds.find(subcommand) != cmds.end())
        {
            result = cmds[subcommand]();
        }
        else
        {
            throw std::invalid_argument("<INVALID SUBARGUMENT>");
        }
    }
    StreamGuard guard(out);
    out << std::fixed << std::setprecision(1);
    out << result << '\n';
}

bool HasNumOfVertexes::operator()(const varfolomeeva::Polygon& poly) const
{
    return poly.points.size() == vertexNum;
}

HasNumOfVertexes::HasNumOfVertexes(size_t num) :
    vertexNum(num)
{}

bool isEven(const varfolomeeva::Polygon& poly)
{
    return (poly.points.size() % 2 == 0);
}

bool isOdd(const varfolomeeva::Polygon& poly)
{
    return (poly.points.size() % 2 != 0);
}

double calcEvenArea(const std::vector< varfolomeeva::Polygon >& poly)
{
    std::vector< varfolomeeva::Polygon > evenPolygons;
    std::copy_if(poly.begin(), poly.end(), std::back_inserter(evenPolygons), isEven);
    return std::accumulate(evenPolygons.begin(), evenPolygons.end(), 0.0, sumArea);
}

double calcOddArea(const std::vector< varfolomeeva::Polygon >& poly)
{
    std::vector< varfolomeeva::Polygon > oddPolygons;
    std::copy_if(poly.begin(), poly.end(), std::back_inserter(oddPolygons), isOdd);
    return std::accumulate(oddPolygons.begin(), oddPolygons.end(), 0.0, sumArea);
}

double calcMeanArea(const std::vector< varfolomeeva::Polygon >& poly)
{
    if (poly.empty())
    {
        throw std::logic_error("<EMPTY POLYGONS>");
    }
    return std::accumulate(poly.begin(), poly.end(), 0.0, sumArea) / poly.size();
}

double calcNumVertexArea(const std::vector< varfolomeeva::Polygon >& poly, size_t vertexNum)
{
    std::vector< varfolomeeva::Polygon > filteredPolygons;
    std::copy_if(poly.begin(), poly.end(),
        std::back_inserter(filteredPolygons), HasNumOfVertexes(vertexNum));
    return std::accumulate(filteredPolygons.begin(), filteredPolygons.end(), 0.0, sumArea);
}

bool compareVertexes(const varfolomeeva::Polygon& p1, const varfolomeeva::Polygon& p2)
{
    return p1.points.size() < p2.points.size();
}

void varfolomeeva::doMax(std::vector< Polygon >& poly, std::istream& in, std::ostream& out)
{
    std::string subcommand;
    in >> subcommand;

    using namespace std::placeholders;
    std::map< std::string, std::function< double() > > cmdsArea;
    cmdsArea["AREA"] = std::bind(getMaxArea, poly);

    std::map< std::string, std::function< double() > > cmdsVert;
    cmdsVert["VERTEXES"] = std::bind(getMaxVertexes, poly);
    StreamGuard guard(out);
    if (cmdsArea.find(subcommand) != cmdsArea.end())
    {
        out << std::fixed << std::setprecision(1);
        out << cmdsArea[subcommand]() << '\n';
    }
    else if (cmdsVert.find(subcommand) != cmdsVert.end())
    {
        out << cmdsVert[subcommand]() << '\n';
    }
    else
    {
        throw std::invalid_argument("<INVALID SUBARGUMENT>");
    }
}

double getMaxArea(const std::vector< varfolomeeva::Polygon >& poly)
{
    if (poly.empty())
    {
        throw std::logic_error("<EMPTY POLYGONS>");
    }
    using namespace std::placeholders;
    return calcArea(*std::max_element(poly.begin(), poly.end(),
        std::bind(std::less< double >(), std::bind(&calcArea, _1), std::bind(&calcArea, _2))));
}

size_t getMaxVertexes(const std::vector< varfolomeeva::Polygon >& poly)
{
    if (poly.empty())
    {
        throw std::logic_error("<EMPTY POLYGONS>");
    }
    return std::max_element(poly.begin(), poly.end(), compareVertexes)->points.size();
}

void varfolomeeva::doMin(std::vector< Polygon >& poly, std::istream& in, std::ostream& out)
{
    std::string subcommand;
    in >> subcommand;

    using namespace std::placeholders;
    std::map< std::string, std::function< double() > > cmdsArea;
    cmdsArea["AREA"] = std::bind(getMinArea, poly);

    std::map< std::string, std::function< double() > > cmdsVert;
    cmdsVert["VERTEXES"] = std::bind(getMinVertexes, poly);
    StreamGuard guard(out);
    if (cmdsArea.find(subcommand) != cmdsArea.end())
    {
        out << std::fixed << std::setprecision(1);
        out << cmdsArea[subcommand]() << '\n';
    }
    else if (cmdsVert.find(subcommand) != cmdsVert.end())
    {
        out << cmdsVert[subcommand]() << '\n';
    }
    else
    {
        throw std::invalid_argument("<INVALID SUBARGUMENT>");
    }
}

double getMinArea(const std::vector< varfolomeeva::Polygon >& poly)
{
    if (poly.empty())
    {
        throw std::logic_error("<EMPTY POLYGONS>");
    }
    using namespace std::placeholders;
    return calcArea(*std::max_element(poly.begin(), poly.end(),
        std::bind(std::less< double >(), std::bind(&calcArea, _1), std::bind(&calcArea, _2))));
}

size_t getMinVertexes(const std::vector< varfolomeeva::Polygon >& poly)
{
    if (poly.empty())
    {
        throw std::logic_error("<EMPTY POLYGONS>");
    }
    return std::min_element(poly.begin(), poly.end(), compareVertexes)->points.size();
}

void varfolomeeva::doCount(std::vector< Polygon >& poly, std::istream& in, std::ostream& out)
{
    size_t result = 0;
    std::string subcommand;
    in >> subcommand;

    std::map< std::string, std::function< size_t() > > cmds;
    using namespace std::placeholders;
    cmds["EVEN"] = std::bind(countEven, poly);
    cmds["ODD"] = std::bind(countOdd, poly);

    size_t vertexNum = 0;
    try
    {
        vertexNum = std::stoull(subcommand);
        if (vertexNum < 3)
        {
            throw std::logic_error("<INCORRECT VERTEX NUMBER>");
        }
        result = countNum(poly, vertexNum);
    }
    catch (const std::invalid_argument&)
    {
        if (cmds.find(subcommand) != cmds.end())
        {
            result = cmds[subcommand]();
        }
        else
        {
            throw std::invalid_argument("<INVALID SUBARGUMENT>");
        }
    }
    out << result << '\n';
}

size_t countEven(const std::vector< varfolomeeva::Polygon >& poly)
{
    return std::count_if(poly.begin(), poly.end(), isEven);
}

size_t countOdd(const std::vector< varfolomeeva::Polygon >& poly)
{
    return std::count_if(poly.begin(), poly.end(), isOdd);
}

size_t countNum(const std::vector< varfolomeeva::Polygon >& poly, size_t vertexNum)
{
    return std::count_if(poly.begin(), poly.end(), HasNumOfVertexes(vertexNum));
}

void varfolomeeva::doRmecho(std::vector< Polygon >& poly, std::istream& in, std::ostream& out)
{
    Polygon subcommand;
    if (!(in >> subcommand))
    {
        throw std::invalid_argument("<INVALID SUBARGUMENT>");
    }
    size_t counter = 0;
    std::vector< Polygon > result;
    auto pred = std::bind(isIdentical, std::placeholders::_1, std::ref(subcommand), std::ref(counter));
    std::copy_if(poly.begin(), poly.end(), std::back_inserter(result), pred);
    out << poly.size() - result.size() << '\n';
    poly = std::move(result);
}

bool isIdentical(const varfolomeeva::Polygon& p1, const varfolomeeva::Polygon& p2, size_t& counter)
{
    if (p1 != p2)
    {
        return true;
    }
    ++counter;
    if (counter > 1)
    {
        counter = 0;
        return false;
    }
    return true;
}

void varfolomeeva::doSame(std::vector< Polygon >& poly, std::istream& in, std::ostream& out)
{
    Polygon subcommand;
    in >> subcommand;
    if (!in || in.peek() != '\n')
    {
        throw std::invalid_argument("<INVALID SUBARGUMENT>");
    }
    auto pred = std::bind(isSame, std::placeholders::_1, subcommand);
    out << count_if(poly.begin(), poly.end(), pred) << '\n';
}

bool isSame(const varfolomeeva::Polygon& p1, const varfolomeeva::Polygon& p2)
{
    if (p1.points.size() != p2.points.size())
    {
        return false;
    }
    int x = p1.points.front().x - p2.points.front().x;
    int y = p1.points.front().y - p2.points.front().y;
    auto pred = std::bind(isSamePoint, std::placeholders::_1, x, y, p2);
    return std::distance(p2.points.begin(), p2.points.end()) == std::count_if(p1.points.begin(), p1.points.end(), pred);
}

bool isSamePoint(const varfolomeeva::Point& point, int x, int y, const varfolomeeva::Polygon& poly)
{
    varfolomeeva::Point dest = { point.x - x, point.y - y };
    return std::find(poly.points.begin(), poly.points.end(), dest) != poly.points.end();
}

bool MaxSeqCounter::operator()(const varfolomeeva::Polygon& poly, const varfolomeeva::Polygon& data)
{
    if (poly == data)
    {
        currentSeqLength++;
        maxSeqLength = std::max(maxSeqLength, currentSeqLength);
    }
    else
    {
        currentSeqLength = 0;
    }
    return maxSeqLength;
}
