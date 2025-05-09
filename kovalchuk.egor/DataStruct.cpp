#include "DataStruct.h"
#include <stdexcept>
#include <cstdlib>

namespace nspace
{
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        char ch = '\0';
        in >> ch;
        if (in && (ch != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DoubleLiteralIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string token;
        in >> token;
        if (token.size() < 2 ||
            (token.back() != 'd' && token.back() != 'D'))
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        token.pop_back();
        try
        {
            dest.ref = std::stod(token);
        }
        catch (const std::exception&)
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in,
        UnsignedLongLongLiteralIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string token;
        in >> token;
        if (token.size() < 3)
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::string suffix = token.substr(token.size() - 3);
        std::string lowerSuffix;
        for (char ch : suffix)
        {
            lowerSuffix.push_back(static_cast<char>(std::tolower(ch)));
        }
        if (lowerSuffix != "ull")
        {
            in.setstate(std::ios::failbit);
            return in;
        }
        token.erase(token.size() - 3);
        try
        {
            dest.ref = std::stoull(token);
        }
        catch (const std::exception&)
        {
            in.setstate(std::ios::failbit);
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
        return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string data;
        if ((in >> data) && (data != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        DataStruct input;
        in >> DelimiterIO{ '(' };
        bool hasKey1 = false;
        bool hasKey2 = false;
        bool hasKey3 = false;
        std::string key;
        while (in >> DelimiterIO{ ':' } >> key)
        {
            if (key == ")")
            {
                in.putback(')');
                break;
            }
            if (key == "key1")
            {
                in >> DoubleLiteralIO{ input.key1 };
                hasKey1 = true;
            }
            else if (key == "key2")
            {
                in >> UnsignedLongLongLiteralIO{ input.key2 };
                hasKey2 = true;
            }
            else if (key == "key3")
            {
                in >> StringIO{ input.key3 };
                hasKey3 = true;
            }
        }
        in >> DelimiterIO{ ')' };
        if (hasKey1 && hasKey2 && hasKey3)
        {
            dest = input;
        }
        else
        {
            in.setstate(std::ios::failbit);
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
        IOFmtGuard guard(out);
        out << "(:key1 " << std::fixed << std::setprecision(1)
            << src.key1;
        out << ":key2 " << src.key2 << "ull";
        out << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    IOFmtGuard::IOFmtGuard(std::basic_ios<char>& stream)
        : streamRef_(stream),
        width_(stream.width()),
        fill_(stream.fill()),
        precision_(stream.precision()),
        fmtFlags_(stream.flags())
    {
    }

    IOFmtGuard::~IOFmtGuard()
    {
        streamRef_.width(width_);
        streamRef_.fill(fill_);
        streamRef_.precision(precision_);
        streamRef_.flags(fmtFlags_);
    }
} // namespace nspace
