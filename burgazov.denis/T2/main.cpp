//Variant 5. key1 = DBL SCI; key2 = CHR LIT
#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <iterator>
#include <iomanip>
#include <limits>
#include <fstream>

struct DataStruct 
{
    double key1;
    char key2;
    std::string key3;
};

struct DelimiterIO
{
    char exp;
};

struct DoubleSciIO
{
    double& ref;
};

struct CharIO
{
    char& ref;
};

struct StringIO
{
    std::string& ref;
};

struct KeyNumberIO
{
    int& ref;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    char temp = '0';
    in >> temp;
    if (in && (temp != dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, KeyNumberIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    in >> DelimiterIO{'k'} >> DelimiterIO{'e'} >> DelimiterIO{'y'} >> dest.ref;
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest)
{
    std::istream::sentry sentry(in);
    if (!sentry)
    {
        return in;
    }
    return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, DoubleSciIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    return in >> dest.ref;
}

std::istream& operator>>(std::istream& in, CharIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    return in >> DelimiterIO{'\''} >> dest.ref >> DelimiterIO{'\''};
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
      return in;
    }
    DataStruct input;
    in >> DelimiterIO{'('} >> DelimiterIO{':'};
    int keyName = 0;
    for (int i = 0; (i < 3) && in; ++i) {
        in >> KeyNumberIO{keyName};
        if (keyName == 1) {
            in >> DoubleSciIO{ input.key1 };
        }
        else if (keyName == 2) {
            in >> CharIO{ input.key2 };
        }
        else if (keyName == 3) {
            in >> StringIO{ input.key3 };
        }
        else {
            in.setstate(std::ios::failbit);
        }
        in >> DelimiterIO{':'};
    }
    in >> DelimiterIO{')'};
    if (in) {
        dest = input;
    }
    return in;
}

class iofmtguard
{
public:
    iofmtguard(std::basic_ios< char >& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {}
    ~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
private:
    std::basic_ios< char >& s_;
    std::streamsize width_;
    char fill_;
    std::streamsize precision_;
    std::basic_ios< char >::fmtflags fmt_;
};

std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
    std::ostream::sentry sentry(out);
    if (!sentry) {
      return out;
    }
    iofmtguard fmtguard(out);
    out << "(:key1 " << std::scientific << std::setprecision(3) << src.key1;
    out << ":key2 '" << src.key2;
    out << "':key3 \"" << src.key3 << "\":)";
    return out;
}

bool compare(const DataStruct& first, const DataStruct& second) {
    if (first.key1 != second.key1) {
        return first.key1 < second.key1;
    }
    else if (first.key2 != second.key2) {
        return first.key2 < second.key2;
    }
    else {
        return first.key3.length() < second.key3.length();
    }
}

int main() {
    std::vector<DataStruct> data;
    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );
    std::sort(
        std::begin(data),
        std::end(data),
        compare
    );
    std::copy(
        std::begin(data),
        std::end(data),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );
}
