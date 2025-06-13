#ifndef MAX_COMMANDS_H
#define MAX_COMMANDS_H
#include <iostream>
#include <functional>
#include <string>
#include <map>
#include "shapes.h"
namespace max
{
    typedef std::vector<Polygon> PolyVec;
    void area(const PolyVec&,std::istream&,std::ostream&);
    void countCmd(const PolyVec&,std::istream&,std::ostream&);
    void maxCmd(const PolyVec&,std::istream&,std::ostream&);
    void minCmd(const PolyVec&,std::istream&,std::ostream&);
    void rects(const PolyVec&,std::istream&,std::ostream&);
    void intersectionsCmd(const PolyVec&,std::istream&,std::ostream&);
}
#endif
