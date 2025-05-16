#include "DataStruct.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    using max::DataStruct;

    std::vector<DataStruct> v;
    std::istream_iterator<DataStruct> it(std::cin), end;
    while (it != end) {
        if (std::cin) {
            v.push_back(*it);
            ++it;
        } else {
            std::cin.clear();
            std::string dummy;
            std::getline(std::cin, dummy);
        }
    }

    std::sort(
        v.begin(), v.end(),
        [] (const max::DataStruct& a, const max::DataStruct& b) {
            if (a.key1 != b.key1)
                return a.key1 < b.key1;
            if (a.key2 != b.key2)
                return a.key2 < b.key2;
            return a.key3.size() < b.key3.size();
        }
    );

    std::ostream_iterator<DataStruct> out_it(std::cout, "\n");
    std::copy(v.begin(), v.end(), out_it);

    return 0;
}