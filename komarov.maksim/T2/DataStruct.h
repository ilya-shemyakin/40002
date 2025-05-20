#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <istream>
#include <ostream>

namespace max {

    struct DataStruct {
        double key1;
        unsigned long long key2;
        std::string key3;
    };

    std::istream& operator>>(std::istream& in,  DataStruct& dst);
    std::ostream& operator<<(std::ostream& out, DataStruct const& src);
    bool cmpDataStruct(const DataStruct& a, const DataStruct& b);

}

#endif
