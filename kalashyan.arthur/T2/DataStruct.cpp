#include "DataStruct.h"
#include <cmath>
#include <iomanip>
#include <limits>

namespace 
{
    const double FLOAT_EPSILON = 1e-10;
    const int IGNORE_COUNT = 2;
}

namespace dataStruct
{
    bool compareData(const DataStruct& left, const DataStruct& right)
    {
        const double leftAbs = std::abs(left.key1);
        const double rightAbs = std::abs(right.key1);
        
        if (std::abs(leftAbs - rightAbs) > FLOAT_EPSILON)
        {
            return leftAbs < rightAbs;
        }

        const double leftRatio = static_cast<double>(left.key2.first) 
            / left.key2.second;
        const double rightRatio = static_cast<double>(right.key2.first) 
            / right.key2.second;
        
        if (std::abs(leftRatio - rightRatio) > FLOAT_EPSILON)
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

        char symbol;
        in >> symbol;

        if (in && (symbol != dest.exp))
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

        double realPart = 0.0;
        double imagPart = 0.0;
        in >> DelimiterIO{'#'} >> DelimiterIO{'c'} >> DelimiterIO{'('}
           >> realPart >> imagPart >> DelimiterIO{')'};

        if (in)
        {
            dest.ref = std::complex<double>(realPart, imagPart);
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
            in.ignore(IGNORE_COUNT);
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
        std::string fieldName;
        while (!hasKey1 || !hasKey2 || !hasKey3)
        {
            if (!in)
            {
                break;
            }

            if (in >> sep{':'} >> fieldName)
            {
                if (fieldName == "key1")
                {
                    in >> cmp{input.key1};
                    hasKey1 = true;
                }
                else if (fieldName == "key2")
                {
                    in >> sep{'('} >> sep{':'} >> sep{'N'}
                       >> ll{input.key2.first} >> sep{':'}
                       >> sep{'D'} >> ull{input.key2.second}
                       >> sep{':'} >> sep{')'};
                    hasKey2 = true;
                }
                else if (fieldName == "key3")
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
        out << "(:key1 #c(" << std::fixed << std::setprecision(1) 
            << src.key1.real() << " " << src.key1.imag() << ")"
            << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)"
            << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    IOFormatGuard::IOFormatGuard(std::basic_ios<char>& stream) :
        stream_(stream),
        width_(stream.width()),
        fill_(stream.fill()),
        precision_(stream.precision()),
        flags_(stream.flags())
    {}

    IOFormatGuard::~IOFormatGuard()
    {
        stream_.width(width_);
        stream_.fill(fill_);
        stream_.precision(precision_);
        stream_.flags(flags_);
    }
}