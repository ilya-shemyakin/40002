#include "DataStruct.h"

namespace dataStruct
{
    bool compareData(const DataStruct& left, const DataStruct& right)
    {
        const double leftAbs = std::abs(left.key1);
        const double rightAbs = std::abs(right.key1);
        
        if (std::abs(leftAbs - rightAbs) > 1e-10)
        {
            return leftAbs < rightAbs;
        }

        const double leftRatio = static_cast<double>(left.key2.first) / left.key2.second;
        const double rightRatio = static_cast<double>(right.key2.first) / right.key2.second;
        
        if (std::abs(leftRatio - rightRatio) > 1e-10)
        {
            return leftRatio < rightRatio;
        }

        return left.key3.length() < right.key3.length();
    }

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        char c;
        in >> c;

        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, ComplexIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        double real = 0.0;
        double imag = 0.0;
        in >> DelimiterIO{'#'} >> DelimiterIO{'c'} >> DelimiterIO{'('}
           >> real >> imag >> DelimiterIO{')'};

        if (in)
        {
            dest.ref = std::complex<double>(real, imag);
        }
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

    std::istream& operator>>(std::istream& in, LongLongIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        in >> dest.ref;
        if (in.peek() == 'l' || in.peek() == 'L')
        {
            in.ignore(2);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> dest.ref;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        DataStruct input;
        using sep = DelimiterIO;
        using cmp = ComplexIO;
        using str = StringIO;
        using ll = LongLongIO;
        using ull = UnsignedLongLongIO;

        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;

        in >> sep{'('};
        std::string label;
        while (!hasKey1 || !hasKey2 || !hasKey3)
        {
            if (!in)
            {
                break;
            }

            if (in >> sep{':'} >> label)
            {
                if (label == "key1")
                {
                    in >> cmp{input.key1};
                    hasKey1 = true;
                }
                else if (label == "key2")
                {
                    in >> sep{'('} >> sep{':'} >> sep{'N'}
                       >> ll{input.key2.first} >> sep{':'}
                       >> sep{'D'} >> ull{input.key2.second}
                       >> sep{':'} >> sep{')'};
                    hasKey2 = true;
                }
                else if (label == "key3")
                {
                    in >> str{input.key3};
                    hasKey3 = true;
                }
                else
                {
                    in.setstate(std::ios::failbit);
                }
            }
        }

        in >> sep{':'} >> sep{')'};
        if (in && hasKey1 && hasKey2 && hasKey3)
        {
            dest = input;
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry)
        {
            return out;
        }

        IOFormatGuard guard(out);
        out << "(:key1 #c(" << src.key1.real() << " " << src.key1.imag() << ")"
            << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)"
            << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    IOFormatGuard::IOFormatGuard(std::basic_ios<char>& s) :
        s_(s),
        width_(s.width(0)),
        fill_(s.fill(' ')),
        precision_(s.precision(6)),
        fmt_(s.flags())
    {}

    IOFormatGuard::~IOFormatGuard()
    {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }
}