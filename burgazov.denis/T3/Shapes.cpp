#include "Shapes.h"
#include "Fmtguard.h"

inline std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    char temp = '0';
    if (in >> temp && (temp != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, Point& point)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }

    Point temp;
    in >> DelimiterIO{ '(' } >> temp.x >> DelimiterIO{ ';' } >> temp.y >> DelimiterIO{ ')' };

    if (!in.fail())
    {
        point = temp;
    }
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& polygon)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    iofmtguard ifmtguard(in);
    in >> std::noskipws;
    Polygon tempPolygon;

    int numberOfVertices = 0;
    in >> numberOfVertices;

    if (numberOfVertices < 3)
    {
        in.setstate(std::ios::failbit);
        return in;
    }

    Point point;
    for (int i = 0; i < numberOfVertices; ++i)
    {

        in >> DelimiterIO{ ' ' };
        in >> point;
        tempPolygon.points.push_back(point);
        if (in.fail() && !in.eof())
        {
            return in;
        }
    }

    if (in.peek() != EOF)
    {
        in >> DelimiterIO{ '\n' };
    }

    if (in)
    {
        polygon = tempPolygon;
    }
    return in;
}
