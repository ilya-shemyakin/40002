#ifndef FILL_VECTOR_OF_SHAPES
#define FILL_VECTOR_OF_SHAPES

#include <vector>
#include <limits>
#include <fstream>
#include <exception>

#include "Shapes.h"

namespace shapes
{
    inline std::vector< Polygon > fillVectorOfShapes(std::string filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            throw std::invalid_argument("Error occurred while opening file. Check that such a file exists");
        }

        std::vector< Polygon > shapes;

        while (!file.eof())
        {
            std::copy
            (
                std::istream_iterator< Polygon >(file),
                std::istream_iterator< Polygon >(),
                std::back_inserter(shapes)
            );
            if (file.fail() && !file.eof())
            {
                file.clear();
                file.ignore(std::numeric_limits< std::streamsize>::max(), '\n');
            }
        }

        file.close();
        return shapes;
    }
}

#endif
