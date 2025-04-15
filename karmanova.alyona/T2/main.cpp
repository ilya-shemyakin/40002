#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <iomanip>
#include <complex>
#include <algorithm>
#include <cctype> 
#include <fstream>

namespace nspace
{
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

    iofmtguard::iofmtguard(std::basic_ios< char >& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {}

    iofmtguard::~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    struct Data
    {
        unsigned long long key1;
        std::complex<double> key2;
        std::string key3;

    };

    struct DelimiterIO
    {
        char exp;
    };

    struct ComplexStruct
    {
        std::complex<double>& ref;
    };

    struct UllStruct
    {
        unsigned long long& ref;
    };

    struct StringKeyString
    {
        std::string& ref;
    };

    struct StringKey {
        std::string& ref;
    };

    struct StringUll {
        std::string& ref;
    };

    struct LabelKey
    {
        std::string exp;
    };

    struct LabelUll
    {
        std::string exp;
    };

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, UllStruct&& dest);
    std::istream& operator>>(std::istream& in, ComplexStruct&& dest);
    std::istream& operator>>(std::istream& in, StringKeyString&& dest);
    std::istream& operator>>(std::istream& in, StringKey&& dest);
    std::istream& operator>>(std::istream& in, StringUll&& dest);
    std::istream& operator>>(std::istream& in, LabelKey&& dest);
    std::istream& operator>>(std::istream& in, LabelUll&& dest);
    std::istream& operator>>(std::istream& in, Data& dest);
    std::istream& operator>>(std::istream& in, std::complex<double>& x);
  
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c = '0';
        in >> c;
        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, LabelKey&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string data = ""; // data = ull:key2


        if ((in >> StringKey{data}) && (data != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, LabelUll&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string data = ""; 
        if (in >> StringUll{ data }) {
            std::transform(data.begin(), data.end(), data.begin(), tolower);
            if (data != dest.exp) {
                in.setstate(std::ios::failbit);
            }
        } 
        else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, UllStruct&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> dest.ref >> LabelUll{ "ull" };
    }
   
    std::istream& operator>>(std::istream& in, ComplexStruct&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        in >> DelimiterIO{ '#' } >> DelimiterIO{ 'c' } >> DelimiterIO{ '(' };
        return in >> dest.ref >> DelimiterIO{ ')' } >> DelimiterIO{ ':' };
    }

    std::istream& operator>>(std::istream& in, StringKeyString&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, StringKey&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in, dest.ref, ' ');
    }

    std::istream& operator>>(std::istream& in, StringUll&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in, dest.ref, ':');
    }

    std::istream& operator>>(std::istream& in, std::complex<double>& x) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        double r = 1.0;
        double im = 1.0;
        in >> r >> im;
        x.real(r);
        x.imag(im);
        return in;
    }
    
    bool checkKey(std::string& strKey, std::vector<int>& key) {
        if (strKey.size() == 4) {
            if (strKey.substr(0, 3) == "key") {
                if (isdigit(strKey[3])) {
                    if (strKey[3] - '0' <= 3) {
                        if (strKey[3] != '0') {
                            if (!key[strKey[3] - '0']) {
                                return true;
                            }
                        }
                    }
                }
            }
        }

        return false;
    }

    std::istream& operator>>(std::istream& in, Data& dest)
    {
        bool flag = true;
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        Data input;
        {
            using sep = DelimiterIO;
            using label = LabelKey;
            using complex = ComplexStruct;
            using ull = UllStruct;
            using str = StringKeyString;
            std::vector<int> key(4, 0);
            in >> sep{ '(' } >> sep{ ':' };
            std::string strKey;
            for (int i = 0; i < 3; ++i) {
              
                in >> strKey;
                if (checkKey(strKey, key)) {
                    key[strKey[3] - '0'] = 1;
                    if (strKey[3] == '1') {
                        in >> UllStruct{ input.key1 };
                    }
                    else if (strKey[3] == '2') {
                        in >> ComplexStruct{ input.key2 };
                    }
                    else {
                        in >> str{ input.key3 };
                    }
                }
                else {
                    in.setstate(std::ios::failbit);
                }
            }
            in >> sep{ ':' } >> sep{ ')' };
        }
        if (in)
        {
            dest = input;
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const Data& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        iofmtguard fmtguard(out);
        out << "(";
        out << ":key1 " << std::fixed << std::setprecision(1) << src.key1 << "ull";
        out << ":key2 #("  << src.key2.real() << " " << src.key2.imag() << ")";
        out << ":key3 \"" << src.key3;
        out << "\":)";
        return out;
    }
}
using nspace::Data;
bool cmp(const Data& first, const Data& other) { // tmp1 <= tmp2
    if (first.key1 < other.key1) {
        return true;
    }
    else if (first.key1 > other.key1) {
        return false;
    }
    else {
        if (std::abs(first.key2) < std::abs(other.key2)) {
            return true;
        }
        else if (std::abs(first.key2) > std::abs(other.key2)) {
            return false;
        }
        else {
            if (first.key3.size() <= other.key3.size()) {
                return true;
            }
            else {
                return false;
            }
        }
    }
}


int main() {
    using nspace::Data;
    std::vector<Data> data;
   
    // std::ifstream file("file.txt");
    while (!std::cin.eof()) {
        std::copy(
            std::istream_iterator< Data >(std::cin),
            std::istream_iterator< Data >(),
            std::back_inserter(data)
        );
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::sort(std::begin(data), std::end(data), cmp);
    std::cout << "Data:\n";
    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator< Data >(std::cout, "\n")
    );
    return 0;
}