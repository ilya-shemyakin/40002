#include "DataStruct.h"
#include <cctype>
#include <iomanip>
#include <iterator>

namespace max
{

iofmtguard::iofmtguard(std::ios & s)
    : s_(s)
    , flags_(s.flags())
    , prec_(s.precision())
    , fill_(s.fill())
{}

iofmtguard::~iofmtguard()
{
    s_.flags(flags_);
    s_.precision(prec_);
    s_.fill(fill_);
}

std::istream & operator>>(std::istream & in, DelimiterIO && d)
{
    std::istream::sentry ok(in);
    if (!ok) return in;
    char c;
    in >> c;
    if (c != d.exp) in.setstate(std::ios::failbit);
    return in;
}

std::istream & operator>>(std::istream & in, DoubleIO && d)
{
    std::istream::sentry ok(in);
    if (!ok) return in;
    double tmp;
    char suf;
    in >> tmp >> suf;
    if (in && (suf == 'd' || suf == 'D')) {
        d.ref = tmp;
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream & operator>>(std::istream & in, ULLIO && d)
{
    std::istream::sentry ok(in);
    if (!ok) return in;
    unsigned long long tmp;
    std::string suf;
    in >> tmp >> suf;
    for (auto & c : suf) {
        c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }
    if (in && suf == "ull") {
        d.ref = tmp;
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream & operator>>(std::istream & in, StringIO && d)
{
    std::istream::sentry ok(in);
    if (!ok) return in;
    char quote;
    in >> quote;
    if (!in || quote != '"') {
        in.setstate(std::ios::failbit);
        return in;
    }
    std::getline(in, d.ref, '"');
    return in;
}

std::istream & operator>>(std::istream & in, LabelIO && d)
{
    std::istream::sentry ok(in);
    if (!ok) return in;
    std::string tok;
    in >> tok;
    if (!in || tok != d.exp) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream & operator>>(std::istream & in, DataStruct & d)
{
    std::istream::sentry ok(in);
    if (!ok) return in;

    DataStruct tmp{};
    bool h1 = false, h2 = false, h3 = false;

    in >> DelimiterIO{'('} >> DelimiterIO{':'};
    while (in) {
        std::string name;
        in >> name;
        if (!in) break;
        if (name == ")") {
            break;
        }

        if (name == "key1") {
            in >> DoubleIO{tmp.key1};
            h1 = bool(in);
        }
        else if (name == "key2") {
            in >> ULLIO{tmp.key2};
            h2 = bool(in);
        }
        else if (name == "key3") {
            in >> StringIO{tmp.key3};
            h3 = bool(in);
        }
        else {
            in.setstate(std::ios::failbit);
        }

        in >> DelimiterIO{':'};
    }
    in >> DelimiterIO{')'};

    if (h1 && h2 && h3) {
        d = tmp;
    } else {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::ostream & operator<<(std::ostream & out, const DataStruct & d)
{
    std::ostream::sentry ok(out);
    if (!ok) return out;
    iofmtguard guard(out);

    out << "(:";
    out << "key1 "
        << std::fixed << std::setprecision(6)
        << d.key1
        << 'd'
        << ':';
    out << "key2 "
        << d.key2
        << "ull"
        << ':';
    out << "key3 \""
        << d.key3
        << "\":";
    out << ')';

    return out;
}

}