#ifndef MAX_DATASTRUCT_HPP
#define MAX_DATASTRUCT_HPP

#include <string>
#include <istream>
#include <ostream>

namespace max
{
    struct DataStruct
    {
        double              key1;
        unsigned long long  key2;
        std::string         key3;
    };

    struct DelimiterIO               { char                exp; };
    struct DoubleLiteralIO           { double            & ref; };
    struct UnsignedLongLongLiteralIO { unsigned long long & ref; };
    struct StringIO                  { std::string       & ref; };
    struct LabelIO                   { const std::string   exp; };

    class IOFmtGuard
    {
    public:
        explicit IOFmtGuard(std::basic_ios<char>& s);
        ~IOFmtGuard();
    private:
        std::basic_ios<char>& stream_;
        std::streamsize        w_, p_;
        char                   fill_;
        std::ios::fmtflags     f_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&&   d);
    std::istream& operator>>(std::istream& in, DoubleLiteralIO&& d);
    std::istream& operator>>(std::istream& in, UnsignedLongLongLiteralIO&& d);
    std::istream& operator>>(std::istream& in, StringIO&& d);
    std::istream& operator>>(std::istream& in, LabelIO&& d);
    std::istream& operator>>(std::istream& in, DataStruct& dst);

    std::ostream& operator<<(std::ostream& out, const DataStruct& src);
}

#endif


