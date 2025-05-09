#ifndef COMMANDS
#define COMMANDS

#include <iostream>
#include <vector>
#include <map>
#include <functional>

#include "Shapes.h"
#include "Subcommands.h"

namespace cmd
{
    void area(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out);
    void max(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out);
    void min(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out);
    void count(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out);
    void inframe(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out);
    void rightshapes(const std::vector< shapes::Polygon >& shapes, std::istream& in, std::ostream& out);
}

#endif
