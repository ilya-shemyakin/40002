#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <string>
#include <iostream>

namespace max
{

    struct DataStruct
    {
        double key1;
        unsigned long long key2;
        std::string key3;
    };

    struct DelimiterIO
    {
        char exp;
    };

    struct DoubleIO
    {
        double & ref;
    };

    struct ULLIO
    {
        unsigned long long & ref;
    };

    struct StringIO
    {
        std::string & ref;
    };

    struct LabelIO
    {
        const char * exp;
    };

    class iofmtguard
    {
    public:
        explicit iofmtguard(std::ios & s);
        ~iofmtguard();

    private:
        std::ios &           s_;
        std::ios::fmtflags   flags_;
        std::streamsize      prec_;
        char                 fill_;
    };

    std::istream & operator>>(std::istream & in, DelimiterIO && d);
    std::istream & operator>>(std::istream & in, DoubleIO && d);
    std::istream & operator>>(std::istream & in, ULLIO && d);
    std::istream & operator>>(std::istream & in, StringIO && d);
    std::istream & operator>>(std::istream & in, LabelIO && d);
    std::istream & operator>>(std::istream & in, DataStruct & d);

    std::ostream & operator<<(std::ostream & out, const DataStruct & d);

}

#endif // DATASTRUCT_H