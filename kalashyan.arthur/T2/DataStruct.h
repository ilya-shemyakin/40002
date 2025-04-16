#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <complex>
#include <string>
#include <iomanip>

namespace dataStruct
{
    struct DataStruct
    {
        std::complex<double> key1;
        std::pair<long long, unsigned long long> key2;
        std::string key3;
    };

    bool compareData(const DataStruct& left, const DataStruct& right);

    struct DelimiterIO
    {
        char exp;
    };

    struct ComplexIO
    {
        std::complex<double>& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LongLongIO
    {
        long long& ref;
    };

    struct UnsignedLongLongIO
    {
        unsigned long long& ref;
    };

    class IOFormatGuard
    {
    public:
        explicit IOFormatGuard(std::basic_ios<char>& s);
        ~IOFormatGuard();
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::ios::fmtflags fmt_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, ComplexIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LongLongIO&& dest);
    std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

#endif