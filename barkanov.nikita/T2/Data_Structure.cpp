#include "Data_Structure.h"

namespace nspace {

    bool comparator(const DataStructure& struct1, const DataStructure& struct2) {
        if (struct1.key1 == struct2.key1 && std::abs(struct1.key2) == std::abs(struct2.key2))
            return struct1.key3.length() < struct2.key3.length();
        if (struct1.key1 == struct2.key1) {
            return std::abs(struct1.key2) < std::abs(struct2.key2);
        }
        return struct1.key1 < struct2.key1;
    }

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, OctalUnsignedLongLongIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> DelimiterIO{ '0' } >> std::oct >> dest.ref;
    }

    std::istream& operator>>(std::istream& in, ComplexDoubleIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        double real = 0.0;
        double img = 0.0;

        in >> DelimiterIO{ '(' } >> real >> img >> DelimiterIO{ ')' };
        if (in)
            dest.ref = std::complex<double>(real, img);

        return in;
    }

    std::istream& operator>> (std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>> (std::istream& in, DataStructure& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        {
            using sep = DelimiterIO;
            using octull = OctalUnsignedLongLongIO;
            using cmp = ComplexDoubleIO;
            using str = StringIO;

            DataStructure input;

            bool flag1 = false, flag2 = false, flag3 = false;
            std::string keyX;
            in >> sep{ '(' };
            while (!flag1 || !flag2 || !flag3) {
                if (!in)
                    break;
                if (in >> keyX) {
                    if (keyX == ":key1") {
                        in >> octull{ input.key1 };
                        flag1 = true;
                    }
                    else if (keyX == ":key2") {
                        in >> sep{ '#' } >> sep{ 'c' } >> cmp{ input.key2 };
                        flag2 = true;
                    }
                    else if (keyX == ":key3") {
                        in >> str{ input.key3 };
                        flag3 = true;
                    }
                    else {
                        in.setstate(std::ios::failbit);
                        break;
                    }
                }
            }
            in >> sep{ ':' } >> sep{ ')' };
            if (in) {
                dest = input;
            }
            return in;
        }
    }

    std::ostream& operator<<(std::ostream& out, const DataStructure& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }
        iofmtguard fmtguard(out);
        out << "(:key1 " << std::oct << src.key1;
        out << ":key2 " << "#c(" << src.key2.real() << " " << src.key2.imag() << ")";
        out << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    iofmtguard::iofmtguard(std::basic_ios< char >& s) :
        s_(s),
        width_(s.width()),
        fill_(s.fill()),
        precision_(s.precision()),
        fmt_(s.flags())
    {
    }
    iofmtguard::~iofmtguard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
}
