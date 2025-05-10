#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iterator>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <cctype>
#include <algorithm>

namespace nspace
{
    struct DataStruct
    {
        double key1;
        long long key2;
        std::string key3;

        bool has_key1 = false;
        bool has_key2 = false;
        bool has_key3 = false;
    };

    struct DelimiterIO
    {
        char exp;
    };

    struct DoubleIO
    {
        double& ref;
    };

    struct LongLongIO
    {
        long long& ref;
    };

    struct StringIO
    {
        std::string& ref;
    };

    struct LabelIO
    {
        std::string exp;
    };


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

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
    std::istream& operator>>(std::istream& in, DoubleIO&& dest);
    std::istream& operator>>(std::istream& in, StringIO&& dest);
    std::istream& operator>>(std::istream& in, LabelIO&& dest);
    std::istream& operator>>(std::istream& in, DataStruct& dest);
    std::ostream& operator<<(std::ostream& out, const DataStruct& dest);
}

int main()
{
    using namespace nspace;

    std::vector<DataStruct> data;

    std::copy(
        std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data)
    );

    std::sort(data.begin(), data.end(), [](const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
        });

    std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));
    std::cout << "Total read: " << data.size() << "\n";

    return 0;
}

namespace nspace
{
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

    std::istream& operator>>(std::istream& in, DoubleIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        in >> dest.ref;
        char suffix;
        in >> suffix;
        if (suffix != 'd' && suffix != 'D')
            in.setstate(std::ios::failbit);
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

    std::istream& operator>>(std::istream& in, LongLongIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        std::string token;
        char ch;

        while (in.get(ch)) {
            if (std::isspace(ch) || ch == ':' || ch == ')') {
                in.unget();
                break;
            }
            token += ch;
        }

        try {
            if (token.size() > 2 &&
                (token.substr(token.size() - 2) == "ll" || token.substr(token.size() - 2) == "LL"))
            {
                token = token.substr(0, token.size() - 2);
                dest.ref = std::stoll(token);
            }
            else {
                in.setstate(std::ios::failbit);
            }
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream& operator>>(std::istream& in, LabelIO&& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        std::string data = "";
        if ((in >> StringIO{ data }) && (data != dest.exp))
        {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest)
    {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct temp;

        if (!(in >> DelimiterIO{ '(' })) {
            return in;
        }

        if (!(in >> DelimiterIO{ ':' })) {
            return in;
        }

        bool ok = true;
        while (ok && in.peek() != ')') {
            std::string label;
            in >> label;

            if (!in) {
                break;
            }

            if (label == "key1") {
                if (!(in >> DoubleIO{ temp.key1 })) {
                    ok = false;
                    break;
                }
                temp.has_key1 = true;
            }
            else if (label == "key2") {
                if (!(in >> LongLongIO{ temp.key2 })) {
                    ok = false;
                    break;
                }
                temp.has_key2 = true;
            }
            else if (label == "key3") {
                if (!(in >> StringIO{ temp.key3 })) {
                    ok = false;
                    break;
                }
                temp.has_key3 = true;
            }
            else {
                ok = false;
                break;
            }

            if (in.peek() != ')') {
                if (!(in >> DelimiterIO{ ':' })) {
                    break;
                }
            }
        }

        if (!(in >> DelimiterIO{ ')' })) {
            in.setstate(std::ios::failbit);
        }

        if (!temp.has_key1 || !temp.has_key2 || !temp.has_key3) {
            in.setstate(std::ios::failbit);
        }
        else {
            dest = temp;
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

        out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d";
        out << ":key2 " << src.key2 << "ll";
        out << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

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
}