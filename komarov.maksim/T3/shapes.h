#ifndef MAX_SHAPES_H
#define MAX_SHAPES_H
#include <vector>
#include <string>
#include <sstream>
#include <numeric>
#include <algorithm>
#include <cmath>
namespace max
{
struct Point { int x; int y; };

struct Polygon
{
    std::vector<Point> points;
    double area() const
    {
        if(points.size() < 3) return 0.0;
        long long s = 0;
        for(std::size_t i = 0; i + 1 < points.size(); ++i)
            s += 1LL*points[i].x*points[i+1].y - 1LL*points[i+1].x*points[i].y;
        s += 1LL*points.back().x*points.front().y
           - 1LL*points.front().x*points.back().y;
        return std::abs(s) / 2.0;
    }
    bool isRectangle() const
    {
        if(points.size() != 4) return false;
        for(std::size_t i = 0; i < 4; ++i)
        {
            Point v1{ points[(i+1)%4].x - points[i].x,
                      points[(i+1)%4].y - points[i].y };
            Point v2{ points[(i+3)%4].x - points[i].x,
                      points[(i+3)%4].y - points[i].y };
            if(v1.x*v2.x + v1.y*v2.y != 0) return false;
        }
        return true;
    }
};

inline int orient(const Point&a,const Point&b,const Point&c)
{
    long long v = 1LL*(b.x-a.x)*(c.y-a.y) - 1LL*(b.y-a.y)*(c.x-a.x);
    return v==0?0:(v>0?1:2);
}
inline bool onSeg(const Point&a,const Point&b,const Point&c)
{
    return std::min(a.x,c.x)<=b.x&&b.x<=std::max(a.x,c.x)&&
           std::min(a.y,c.y)<=b.y&&b.y<=std::max(a.y,c.y);
}
inline bool segInter(const Point&p1,const Point&q1,const Point&p2,const Point&q2)
{
    int o1=orient(p1,q1,p2), o2=orient(p1,q1,q2),
        o3=orient(p2,q2,p1), o4=orient(p2,q2,q1);
    if(o1!=o2 && o3!=o4) return true;
    if(o1==0 && onSeg(p1,p2,q1)) return true;
    if(o2==0 && onSeg(p1,q2,q1)) return true;
    if(o3==0 && onSeg(p2,p1,q2)) return true;
    if(o4==0 && onSeg(p2,q1,q2)) return true;
    return false;
}
inline bool pointInPoly(const Polygon&poly,const Point&p)
{
    bool inside=false;
    for(std::size_t i=0,j=poly.points.size()-1;i<poly.points.size();j=i++)
    {
        const Point&pi=poly.points[i];
        const Point&pj=poly.points[j];
        bool cross=((pi.y>p.y)!=(pj.y>p.y)) &&
                    (p.x < (pj.x-pi.x)*1.0*(p.y-pi.y)/(pj.y-pi.y)+pi.x);
        if(cross) inside=!inside;
    }
    return inside;
}
inline bool intersects(const Polygon&a,const Polygon&b)
{
    for(std::size_t i=0;i<a.points.size();++i)
    {
        Point a1=a.points[i],a2=a.points[(i+1)%a.points.size()];
        for(std::size_t j=0;j<b.points.size();++j)
        {
            Point b1=b.points[j],b2=b.points[(j+1)%b.points.size()];
            if(segInter(a1,a2,b1,b2)) return true;
        }
    }
    return pointInPoly(a,b.points[0]) || pointInPoly(b,a.points[0]);
}

inline bool parsePolygon(const std::string& line, Polygon& out)
{
    std::istringstream iss(line);
    std::size_t n;
    if(!(iss>>n) || n < 3) return false;
    Polygon tmp; tmp.points.reserve(n);
    char c;
    for(std::size_t i=0;i<n;++i)
    {
        if(!(iss>>c) || c!='(') return false;
        Point p{};
        if(!(iss>>p.x)) return false;
        if(!(iss>>c) || c!=';') return false;
        if(!(iss>>p.y)) return false;
        if(!(iss>>c) || c!=')') return false;
        tmp.points.push_back(p);
    }
    char extra;
    if(iss >> extra) return false;
    out = std::move(tmp);
    return true;
}
}
#endif

