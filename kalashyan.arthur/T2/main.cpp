#include "DataStruct.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>

int main()
{
    using dataStruct::DataStruct;
    std::vector<DataStruct> dataVector;

    while (!std::cin.eof())
    {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(dataVector)
        );

        if (!std::cin && !std::cin.eof())
        {
            std::cin.clear();
            constexpr auto maxIgnore = std::numeric_limits<std::streamsize>::max();
            std::cin.ignore(maxIgnore, '\n');
        }
    }

    std::sort(
        dataVector.begin(),
        dataVector.end(),
        dataStruct::compareData);
    std::copy(
        dataVector.begin(),
        dataVector.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}
