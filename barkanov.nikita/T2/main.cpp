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
    std::ifstream fileInput("test_input.txt");
    if (!fileInput) {
        std::cerr << "Unable to open file test_input.txt\n";
        return 1;
    }

    return 0;
}

