#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <cctype>
#include <iomanip>
#include <limits>

namespace nspace {

    struct DataStruct {
        unsigned long long key1; // ULL LIT
        unsigned long long key2; // ULL HEX
        std::string key3;
    };

    struct DelimiterIO { char exp; };
    struct ULLLitIO { unsigned long long& value; };
    struct ULLHexIO { unsigned long long& value; };
    struct StringIO { std::string& ref; };

    class iofmtguard {
    public:
        iofmtguard(std::basic_ios<char>& s);
        ~iofmtguard();
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s), width_(s.width()), fill_(s.fill()),
        precision_(s.precision()), fmt_(s.flags()) {
    }

    iofmtguard::~iofmtguard() {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        char c;
        in >> c;
        if (c != dest.exp) in.setstate(std::ios::failbit);
        return in;
    }

    std::istream& operator>>(std::istream& in, ULLLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        std::string str;
        char c;
        while (in.get(c) && c != ':') str += c;
        if (c == ':') in.putback(c);
        if (str.size() < 3) {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::string suffix = str.substr(str.size() - 3);
        for (char& ch : suffix) ch = std::tolower(ch);
        if (suffix != "ull") {
            in.setstate(std::ios::failbit);
            return in;
        }
        try {
            dest.value = std::stoull(str.substr(0, str.size() - 3));
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, ULLHexIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        std::string str;
        char c;
        while (in.get(c) && c != ':') str += c;
        if (c == ':') in.putback(c);
        if (str.size() < 2 || (str.substr(0, 2) != "0x" && str.substr(0, 2) != "0X")) {
            in.setstate(std::ios::failbit);
            return in;
        }
        try {
            dest.value = std::stoull(str.substr(2), nullptr, 16);
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        char quote;
        in >> quote;
        if (quote != '"') {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::getline(in, dest.ref, '"');
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        DataStruct tmp;
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

        while (true) {
            char c;
            if (!in.get(c)) break;
            if (c == ':') {
                if (in.peek() == ')') {
                    in.get();
                    break;
                }
                in.putback(c);
                std::string key;
                in >> DelimiterIO{ ':' } >> key >> DelimiterIO{ ' ' };
                if (key == "key1") {
                    if (!(in >> ULLLitIO{ tmp.key1 })) break;
                    hasKey1 = true;
                }
                else if (key == "key2") {
                    if (!(in >> ULLHexIO{ tmp.key2 })) break;
                    hasKey2 = true;
                }
                else if (key == "key3") {
                    if (!(in >> StringIO{ tmp.key3 })) break;
                    hasKey3 = true;
                }
                else {
                    in.setstate(std::ios::failbit);
                    break;
                }
                in >> DelimiterIO{ ':' };
            }
            else {
                in.putback(c);
                in.setstate(std::ios::failbit);
                break;
            }
        }

        if (in && hasKey1 && hasKey2 && hasKey3) dest = tmp;
        else in.setstate(std::ios::failbit);

        if (in.fail()) {
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '(');
        }
        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
        iofmtguard guard(out);
        out << "(:key1 " << data.key1 << "ull:key2 0x" << std::uppercase << std::hex << data.key2
            << std::dec << ":key3 \"" << data.key3 << "\":)";
        return out;
    }

} // namespace nspace

bool compare(const nspace::DataStruct& a, const nspace::DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
}

int main() {
    std::vector<nspace::DataStruct> data;

    std::copy(std::istream_iterator<nspace::DataStruct>(std::cin),
        std::istream_iterator<nspace::DataStruct>(),
        std::back_inserter(data));

    std::sort(data.begin(), data.end(), compare);

    std::copy(data.begin(), data.end(),
        std::ostream_iterator<nspace::DataStruct>(std::cout, "\n"));

    return 0;
}
