#include "DataStruct.h"

int main()
{
    using nspace::DataStruct;

    std::vector<DataStruct> data;
    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), [](const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.size() < b.key3.size();
        });


    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );


    std::cout
        << "good: " << std::cin.good() << ", "
        << "fail: " << std::cin.fail() << ", "
        << "bad: " << std::cin.bad() << ", "
        << "eof: " << std::cin.eof() << std::endl;
    return 0;
}
