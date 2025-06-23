#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <complex>
#include <utility>
#include <cctype>
#include <iomanip>
#include <iterator>

namespace nspace
{
    struct DataStruct
    {
        long long key1;
        std::pair<long long, unsigned long long> key2;
        std::string key3;
    };

    struct DelimiterIO
    {
        char exp;
    };

    struct LongLongIO
    {
        long long& ref;
    };

    struct UnsignedLongLongIO
    {
        unsigned long long& ref;
    };

    struct PairIO
    {
        std::pair<long long, unsigned long long>& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LabelIO
    {
        std::string exp;
    };

    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios<char>& s);
        ~iofmtguard();
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, LongLongIO&& dest);
    std::istream& operator>>(std::istream& in, PairIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

#endif //!DATASTRUCT_H
