#include "DataStruct.h"

static const int NEXT_CHAR_AS_LOWERCASE_DATA_ID = std::ios::xalloc();

namespace dataStruct
{
    unsigned long long gcd(unsigned long long a, unsigned long long b)
    {
        if (a < b)
        {
            std::swap(a, b);
        }
        unsigned long long temp = 0;
        while (b != 0)
        {
            temp = b;
            b = a % b;
            a = temp;
        }
        return a;
    }

    bool comparator(const DataStruct& left, const DataStruct& right)
    {
        if (left.key1 == right.key1 && left.key2 == right.key2)
        {
            return left.key3 < right.key3;
        }
        if (left.key1 == right.key1)
        {
            unsigned long long greatestCommonDivisor = dataStruct::gcd(left.key2.second, right.key2.second);
            return left.key2.first * static_cast<long long>(right.key2.second / greatestCommonDivisor) <\
               right.key2.first * static_cast<long long>(left.key2.second / greatestCommonDivisor);
        }
        return left.key1 < right.key1;

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char c = '0';
        if (in.iword(NEXT_CHAR_AS_LOWERCASE_DATA_ID) == 1)
        {
            in >> c;
            c = std::tolower(c);
            in.iword(NEXT_CHAR_AS_LOWERCASE_DATA_ID) = 0;
        }
        else
        {
            in >> c;
        }
        if (in && (c != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> dest.ref >> nextCharAsLowercase >> DelimiterIO{ 'd' };
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LongLongIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' }\
            >> dest.ref >> DelimiterIO{ ':' };
    }

    std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> DelimiterIO{ 'D' } >> dest.ref \
            >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
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
        using dbl = DoubleIO;
        using str = StringIO;
        using ll = LongLongIO;
        using ull = UnsignedLongLongIO;
        bool flag1 = false;
        bool flag2 = false;
        bool flag3 = false;
        in >> sep{ '(' };
        std::string label;
        while (!flag1 || !flag2 || !flag3)
        {
            if (!in)
            {
                break;
            }
            if (in >> label)
            {
                if (label == ":key1")
                {
                    in >> dbl{ input.key1 };
                    flag1 = true;
                }
                else if (label == ":key2")
                {
                    in >> ll{ input.key2.first } >> ull{ input.key2.second };
                    flag2 = true;
                }
                else if (label == ":key3")
                {
                    in >> str{ input.key3 };
                    flag3 = true;
                }
                else
                {
                    in.setstate(std::ios::failbit);
                    break;
                }
            }
        }
        in >> sep{ ':' } >> sep{ ')' };
        if (in)
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
        iofmtguard fmtguard(out);
        out << "(";
        out << ":key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
        out << ":key2 " << "(:N " << src.key2.first << ":D " << src.key2.second << ":)";
        out << ":key3 \"" << src.key3 << "\":";
        out << ")";
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

    std::istream& nextCharAsLowercase(std::istream& is)
    {
        is.iword(NEXT_CHAR_AS_LOWERCASE_DATA_ID) = 1;
        return is;
    }
}
