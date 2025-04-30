#include <iostream>
#include "DataStructure.h"
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <vector>
#include <iterator>
int main() {
    using nspace::DataStructure;
    std::vector<DataStructure> data;
    while (!std::cin.eof()) {
        std::copy(
            std::istream_iterator< DataStructure >(std::cin),
            std::istream_iterator< DataStructure >(),
            std::back_inserter(data)
        );
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::sort(std::begin(data), std::end(data), nspace::comparator);
    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator< DataStructure >(std::cout, "\n")
    );
    return 0;
}