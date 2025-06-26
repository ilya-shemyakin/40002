#include "Geometry.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <numeric>
#include <iomanip>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Error: Filename required\n";
        return 1;
    }

    std::ifstream fin(argv[1]);
    if (!fin)
    {
        std::cerr << "Error: Cannot open file\n";
        return 1;
    }

    std::vector<Polygon> polygons;
    std::string line;
    while (std::getline(fin, line))
    {
        if (line.empty())
        {
            continue;
        }
        Polygon poly;
        if (readPolygon(line, poly))
        {
            polygons.push_back(std::move(poly));
        }
    }
    fin.close();

    std::cout << std::fixed << std::setprecision(1);
    while (std::getline(std::cin, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::istringstream iss(line);
        std::string cmd, arg;
        iss >> cmd;

        if (cmd == "AREA")
        {
            iss >> arg;
            if (arg == "EVEN" || arg == "ODD")
            {
                bool wantEven = arg == "EVEN";
                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [wantEven](double acc, const Polygon& p)
                    {
                        return ((p.points.size() % 2 == 0) == wantEven)
                            ? acc + calculateArea(p) : acc;
                    });
                std::cout << sum << "\n";
            }
            else if (arg == "MEAN")
            {
                if (polygons.empty())
                {
                    std::cout << "<INVALID COMMAND>\n";
                    continue;
                }
                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                    [](double acc, const Polygon& p)
                    {
                        return acc + calculateArea(p);
                    });
                std::cout << sum / polygons.size() << "\n";
            }
            else
            {
                try
                {
                    size_t v = std::stoul(arg);
                    double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                        [v](double acc, const Polygon& p)
                        {
                            return p.points.size() == v ? acc + calculateArea(p) : acc;
                        });
                    std::cout << sum << "\n";
                }
                catch (...)
                {
                    std::cout << "<INVALID COMMAND>\n";
                }
            }
        }
        else if (cmd == "MAX" || cmd == "MIN")
        {
            if (polygons.empty())
            {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }
            iss >> arg;
            bool isMax = cmd == "MAX";
            if (arg == "AREA")
            {
                auto comp = [](const Polygon& a, const Polygon& b)
                    {
                        return calculateArea(a) < calculateArea(b);
                    };
                auto it = isMax
                    ? std::max_element(polygons.begin(), polygons.end(), comp)
                    : std::min_element(polygons.begin(), polygons.end(), comp);
                std::cout << calculateArea(*it) << "\n";
            }
            else if (arg == "VERTEXES")
            {
                auto comp = [](const Polygon& a, const Polygon& b)
                    {
                        return a.points.size() < b.points.size();
                    };
                auto it = isMax
                    ? std::max_element(polygons.begin(), polygons.end(), comp)
                    : std::min_element(polygons.begin(), polygons.end(), comp);
                std::cout << it->points.size() << "\n";
            }
            else
            {
                std::cout << "<INVALID COMMAND>\n";
            }
        }
        else if (cmd == "COUNT")
        {
            iss >> arg;
            if (arg == "EVEN" || arg == "ODD")
            {
                bool wantEven = arg == "EVEN";
                int cnt = std::count_if(polygons.begin(), polygons.end(),
                    [wantEven](const Polygon& p)
                    {
                        return (p.points.size() % 2 == 0) == wantEven;
                    });
                std::cout << cnt << "\n";
            }
            else
            {
                try
                {
                    size_t v = std::stoul(arg);
                    int cnt = std::count_if(polygons.begin(), polygons.end(),
                        [v](const Polygon& p)
                        {
                            return p.points.size() == v;
                        });
                    std::cout << cnt << "\n";
                }
                catch (...)
                {
                    std::cout << "<INVALID COMMAND>\n";
                }
            }
        }
        else if (cmd == "LESSAREA")
        {
            int n;
            iss >> n;
            if (n < 3)
            {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            Polygon q;
            q.points.reserve(n);
            std::vector<std::string> tokens;
            std::copy(std::istream_iterator<std::string>(iss),
                std::istream_iterator<std::string>(),
                std::back_inserter(tokens));

            if (tokens.size() != static_cast<size_t>(n))
            {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            bool valid = std::all_of(tokens.begin(), tokens.end(),
                [&q](const std::string& tok)
                {
                    Point pt;
                    if (!parsePoint(tok, pt))
                    {
                        return false;
                    }
                    q.points.push_back(pt);
                    return true;
                });

            if (!valid)
            {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            double a0 = calculateArea(q);
            int cnt = std::count_if(polygons.begin(), polygons.end(),
                [a0](const Polygon& p)
                {
                    return calculateArea(p) < a0;
                });
            std::cout << cnt << "\n";
        }
        else if (cmd == "INTERSECTIONS")
        {
            int n;
            iss >> n;
            if (n < 3)
            {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            Polygon q;
            q.points.reserve(n);
            std::vector<std::string> tokens;
            std::copy(std::istream_iterator<std::string>(iss),
                std::istream_iterator<std::string>(),
                std::back_inserter(tokens));

            if (tokens.size() != static_cast<size_t>(n))
            {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            bool valid = std::all_of(tokens.begin(), tokens.end(),
                [&q](const std::string& tok)
                {
                    Point pt;
                    if (!parsePoint(tok, pt))
                    {
                        return false;
                    }
                    q.points.push_back(pt);
                    return true;
                });

            if (!valid)
            {
                std::cout << "<INVALID COMMAND>\n";
                continue;
            }

            Rect r0(q);
            int cnt = std::count_if(polygons.begin(), polygons.end(),
                [r0](const Polygon& p)
                {
                    return Rect(p).intersects(r0);
                });
            std::cout << cnt << "\n";
        }
        else
        {
            std::cout << "<INVALID COMMAND>\n";
        }
    }

    return 0;
}
