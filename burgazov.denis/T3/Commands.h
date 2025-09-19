#ifndef COMMANDS
#define COMMANDS

#include <vector>
#include "Shapes.h"
#include "Subcommands.h"

void area(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
void max(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
void min(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
void count(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
void inframe(const std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);
void echo(std::vector< Polygon >& shapes, std::istream& in, std::ostream& out);

#endif
