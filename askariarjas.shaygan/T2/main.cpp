#include "DataStruct.h"
#include <vector>
#include <algorithm>
#include <iterator>
#include <limits>

int main()
{
    std::vector<dataStruct::DataStruct> data;

    while (std::cin.peek() != EOF)
    {
        dataStruct::DataStruct temp;
        if (std::cin >> temp)
        {
            data.push_back(temp);
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end(), dataStruct::compareData);

    std::copy(data.begin(), data.end(),
        std::ostream_iterator<dataStruct::DataStruct>(std::cout, "\n"));
}
