#include "DataStruct.h"
#include <vector>
#include <algorithm>
#include <iterator>

int main()
{
    using dataStruct::DataStruct;
    std::vector<DataStruct> data;

    while (!std::cin.eof())
    {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );

        if (!std::cin && !std::cin.eof())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::ranges::sort(data, dataStruct::compareData);
    std::ranges::copy(data,std::ostream_iterator<DataStruct>(std::cout, "\n"));

    return 0;
}