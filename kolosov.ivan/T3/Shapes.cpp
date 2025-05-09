#include "Shapes.h"
#include "IOFmtguard.h"
#include "DelimiterIO.h"

const int MIN_AMOUNT_OF_VERTEXES = 3;

namespace shapes
{
    std::istream& operator>>(std::istream& in, Point& point)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        Point input;
        in >> DelimiterIO{ '(' } >> input.x >> DelimiterIO{ ';' } >> input.y >> DelimiterIO{ ')' };

        if (!in.fail())
        {
            point = input;
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

        Polygon input;
        in >> std::noskipws;

        int amountOfVertexes = 0;
        in >> amountOfVertexes;

        if (amountOfVertexes < MIN_AMOUNT_OF_VERTEXES)
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        Point point;
        for (int i = 0; i < amountOfVertexes; ++i)
        {
            if (in.peek() == '\n')
            {
                in.setstate(std::ios::failbit);
                return in;
            }
            in >> DelimiterIO{ ' ' };
            if (in.peek() == '\n')
            {
                in.setstate(std::ios::failbit);
                return in;
            }
            in >> point;
            input.points.push_back(point);
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
            polygon = input;
        }
        return in;
    }
}
