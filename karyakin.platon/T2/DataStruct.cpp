#include "DataStruct.h"


namespace nspace
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, LongLongIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        return in >> dest.ref;
    }

    std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        return in >> dest.ref;
    }

    std::istream& operator>>(std::istream& in, PairIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };
        in >> DelimiterIO{ 'N' } >> LongLongIO{ dest.ref.first } >> DelimiterIO{ ':' };
        in >> DelimiterIO{ 'D' } >> UnsignedLongLongIO{ dest.ref.second };
        in >> DelimiterIO{ ':' } >> DelimiterIO{ ')' };
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        std::string data;
        if ((in >> data) && (data != dest.exp)) in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        DataStruct input;
        in >> DelimiterIO{ '(' };
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
        std::string key;


        while (in >> DelimiterIO{ ':' } && in >> key)
           {
               if (key == ")") {
                   in.putback(')');
                   break;
                }
               if (key == "key1") {
                    in >> LongLongIO{ input.key1 };
                    hasKey1 = true;
                }
               else if (key == "key2") {
                    in >> PairIO{ input.key2 };
                    hasKey2 = true;
                }
               else if (key == "key3") {
                    in >> StringIO{ input.key3 };
                    hasKey3 = true;
                }
            }
        in >> DelimiterIO{ ')' };
        if (hasKey1 && hasKey2 && hasKey3) dest = input;
        else in.setstate(std::ios::failbit);
        return in;

    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src)
    {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;
        iofmtguard fmtguard(out);
        out << "(:key1 " << src.key1;
        out << ":key2 (:N " << src.key2.first << ":D " << src.key2.second << ":)";
        out << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
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
}