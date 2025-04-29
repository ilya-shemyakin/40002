#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H


#include <iostream>
#include <string>
#include <cassert>
#include <iomanip>
#include <complex>

namespace nspace
{

    struct DataStructure
    {
        unsigned long long key1;
        std::complex<double> key2;
        std::string key3;
    };

    bool comparator(const DataStructure& struct1, const DataStructure& struct2);

    struct DelimiterIO
    {
        char exp;
    };

    struct OctalUnsignedLongLongIO
    {
        unsigned long long& ref;
    };

    struct ComplexDoubleIO {
        std::complex<double>& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    //

    // scope guard для возврата состояния потока в первоначальное состояние
    class iofmtguard
    {
    public:
        iofmtguard(std::basic_ios< char >& s);
        ~iofmtguard();
    private:
        std::basic_ios< char >& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios< char >::fmtflags fmt_;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, OctalUnsignedLongLongIO&& dest);
    std::istream& operator>>(std::istream& in, ComplexDoubleIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);

    std::istream& operator>>(std::istream& in, DataStructure& dest);
    std::ostream& operator<<(std::ostream& out, const DataStructure& dest);
}


#endif