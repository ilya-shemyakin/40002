#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <limits>
#include "DataStruct.h"

int main()
{
    using max::DataStruct;

    std::vector<DataStruct> data;

    while (true)
    {
        DataStruct tmp;
        if (std::cin >> tmp)
        {
            data.push_back(tmp);
        }
        else if (std::cin.eof())
        {
            break;
        }
        else
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end(),
              [](const DataStruct& a, const DataStruct& b)
              {
                if (a.key1 != b.key1) return a.key1 < b.key1;
                if (a.key2 != b.key2) return a.key2 < b.key2;
                return a.key3.size() < b.key3.size();
              });

    std::copy(data.begin(), data.end(),
              std::ostream_iterator<DataStruct>(std::cout, "\n"));
}


