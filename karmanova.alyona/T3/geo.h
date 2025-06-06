#ifndef GEO_H
#define GEO_H
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
class iofmtguard
{
public:
    iofmtguard(std::basic_ios< char >& s);
    ~iofmtguard();
private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
};

iofmtguard::iofmtguard(std::basic_ios< char >& s) :
    s_(s),
    width_(s.width()),
    fill_(s.fill()),
    precision_(s.precision()),
    fmt_(s.flags())
{}

iofmtguard::~iofmtguard()
{
    s_.width(width_);
    s_.fill(fill_);
    s_.precision(precision_);
    s_.flags(fmt_);
}


struct DelimiterIO
{
    char exp;
};
struct Point
{
    Point(int x1, int y1) : x(x1), y(y1) {};
    int x, y;
    Point() : x(0), y(0) {};
    bool operator==(const Point& p) const {
        return ((x == p.x) && (y == p.y));
    }

    bool operator<(const Point& p) const {
        if (x < p.x) {
            return true;
        }
        else {
            return (x == p.x) && (y < p.y);
        }
    }

    std::pair<int, int> makeNormal() const {
        return std::minmax(x, y);
    }
};
struct Polygon
{
    std::vector< Point > points;
};


std::istream& operator>>(std::istream& in, Point& point);
std::ostream& operator<<(std::ostream& out, const Point& point);
std::istream& operator>>(std::istream& in, Polygon& figure);
std::istream& operator>>(std::istream& in, DelimiterIO&& dest);

std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    char c = '0';
    in >> c;
    if (in && (c != dest.exp))
    {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, Point& point) {
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    Point tmp;
    in >> DelimiterIO{ '(' } >> tmp.x >> DelimiterIO{ ';' } >> tmp.y >> DelimiterIO{ ')' };
    if (!in.fail()) {
        point = tmp;
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Point& point) {
    return out << '(' << point.x << ';' << point.y << ')';
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

    int size;
    in >> size;
    if (in.fail() || size < 3) // доп. защита
    {
        in.setstate(std::ios::failbit);
        return in;
    }
    Point point;
    for (int i = 0; i < size; ++i)
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

//std::ostream& operator<<(std::ostream& out,const Polygon& figure) {
//    for (int i = 0; i < figure.points.size(); i++) {
//        out << figure.points[i] << " ";
//    }
//    return out;
//}


#endif // !GEO_H


