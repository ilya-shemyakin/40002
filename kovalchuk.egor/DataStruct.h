#pragma once
#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
#include <iterator>
#include <cctype>

namespace nspace
{
    struct DataStruct
    {
        double             key1;
        unsigned long long key2;
        std::string        key3;
    };

    struct DelimiterIO
    {
        char               exp;
    };

    struct DoubleLiteralIO
    {
        double& ref;
    };

    struct UnsignedLongLongLiteralIO
    {
        unsigned long long& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LabelIO
    {
        std::string        exp;
    };

    class IOFmtGuard
    {
    public:
        IOFmtGuard(std::basic_ios<char>& stream);
        ~IOFmtGuard();

    private:
        std::basic_ios<char>& streamRef_;
        std::streamsize       width_;
        char                  fill_;
        std::streamsize       precision_;
        std::ios::fmtflags    fmtFlags_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleLiteralIO&& dest);
    std::istream& operator>>(std::istream& in,
        UnsignedLongLongLiteralIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);

    std::ostream& operator<<(std::ostream& out, const DataStruct& src);
}

#endif // DATASTRUCT_H
