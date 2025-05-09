#ifndef DELIMITER_IO
#define DELIMITER_IO

#include <iostream>

struct DelimiterIO
{
    char exp;
};

inline std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    char c = '0';
    if (in >> c && (c != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

#endif
