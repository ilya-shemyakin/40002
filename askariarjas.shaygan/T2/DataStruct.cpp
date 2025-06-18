#include "DataStruct.h"

namespace dataStruct
{
    bool compareData(const DataStruct& left, const DataStruct& right)
    {
        if (left.key1 != right.key1)
        {
            return left.key1 < right.key1;
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

    std::istream& operator>>(std::istream& in, CharIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        char quote;
        in >> quote;
        if (quote != '\'')
        {
            in.setstate(std::ios::failbit);
            return in;
        }

        in >> dest.ref >> DelimiterIO{ '\'' };
        return in;
    }

    std::istream& operator>>(std::istream& in, RationalIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' } >> DelimiterIO{ 'N' }
            >> dest.ref.first >> DelimiterIO{ ':' }
            >> DelimiterIO{ 'D' } >> dest.ref.second
            >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
        return in;
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

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }

        DataStruct input;
        using sep = DelimiterIO;
        using chr = CharIO;
        using rat = RationalIO;
        using str = StringIO;

        bool flagKey1 = false;
        bool flagKey2 = false;
        bool flagKey3 = false;

        in >> sep{ '(' };
        std::string label;
        while (!flagKey1 || !flagKey2 || !flagKey3)
        {
            if (!in)
            {
                break;
            }

            if (in >> sep{ ':' } >> label)
            {
                if (label == "key1")
                {
                    in >> chr{ input.key1 };
                    flagKey1 = true;
                }
                else if (label == "key2")
                {
                    in >> rat{ input.key2 };
                    flagKey2 = true;
                }
                else if (label == "key3")
                {
                    in >> str{ input.key3 };
                    flagKey3 = true;
                }
                else
                {
                    in.setstate(std::ios::failbit);
                }
            }
        }

        in >> sep{ ':' } >> sep{ ')' };
        if (in && flagKey1 && flagKey2 && flagKey3)
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

        out << "(:key1 '" << src.key1 << "'"
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
