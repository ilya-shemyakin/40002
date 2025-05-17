#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <limits>

struct DataStruct {
    unsigned long long key1_;
    unsigned long long key2_;
    std::string key3_;
};

class IofmtGuard {
public:
    IofmtGuard(std::basic_ios<char>& stream) :
        stream_(stream),
        flags_(stream.flags())
    {
    }
    ~IofmtGuard() { stream_.flags(flags_); }

private:
    std::basic_ios<char>& stream_;
    std::ios_base::fmtflags flags_;
};

struct DelimiterIO {
    char exp;
};

struct UnsignedLongLongIO {
    unsigned long long& ref;
};

struct StringIO {
    std::string& ref;
};

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    char c;
    in >> c;

    if (in && (c != dest.exp)) {
        in.setstate(std::ios::failbit);
    }
    return in;
}

std::istream& operator>>(std::istream& in, UnsignedLongLongIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    in >> dest.ref;
    if (in) {
        char suffix[3];
        if (in >> suffix[0] && in >> suffix[1] && in >> suffix[2] &&
            (suffix[0] == 'u' || suffix[0] == 'U') &&
            (suffix[1] == 'l' || suffix[1] == 'L') &&
            (suffix[2] == 'l' || suffix[2] == 'L')) {
        }
        else {
            in.setstate(std::ios::failbit);
        }
    }
    return in;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }
    return std::getline(in >> DelimiterIO{ '"' }, dest.ref, '"');
}

std::istream& operator>>(std::istream& in, std::pair<char, UnsignedLongLongIO>&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    in >> DelimiterIO{ '0' } >> DelimiterIO{ dest.first };
    if (in) {
        in >> std::hex >> dest.second.ref >> std::dec;
    }
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) {
        return in;
    }

    DataStruct input;
    using sep = DelimiterIO;
    using ull = UnsignedLongLongIO;
    using str = StringIO;

    bool keysPresent[3] = { false, false, false };

    in >> sep{ '(' };
    std::string label;
    while (in && (!keysPresent[0] || !keysPresent[1] || !keysPresent[2])) {
        if (!(in >> sep{ ':' } >> label)) {
            in.setstate(std::ios::failbit);
            break;
        }

        if (label == "key1" && !keysPresent[0]) {
            in >> ull{ input.key1_ };
            keysPresent[0] = true;
        }
        else if (label == "key2" && !keysPresent[1]) {
            in >> std::pair<char, ull>{'x', ull{ input.key2_ }};
            keysPresent[1] = true;
        }
        else if (label == "key3" && !keysPresent[2]) {
            in >> str{ input.key3_ };
            keysPresent[2] = true;
        }
        else {
            in.setstate(std::ios::failbit);
            break;
        }
    }

    if (in && keysPresent[0] && keysPresent[1] && keysPresent[2]) {
        in >> sep{ ')' };
        if (in) {
            dest = input;
        }
    }
    else {
        in.setstate(std::ios::failbit);
    }

    return in;
}

std::ostream& operator<<(std::ostream& output, const DataStruct& data) {
    std::ostream::sentry sentry(output);
    if (!sentry) {
        return output;
    }

    IofmtGuard guard(output);
    output << "(:key1 " << data.key1_ << "ull:key2 0x" << std::hex << std::uppercase
        << data.key2_ << std::dec << ":key3 \"" << data.key3_ << "\":)";
    return output;
}

bool compareData(const DataStruct& first, const DataStruct& second) {
    if (first.key1_ != second.key1_) {
        return first.key1_ < second.key1_;
    }
    if (first.key2_ != second.key2_) {
        return first.key2_ < second.key2_;
    }
    return first.key3_.length() < second.key3_.length();
}

int main() {
    std::vector<DataStruct> ds;
    while (!std::cin.eof()) {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(ds)
        );
        if (!std::cin.fail()) {
            continue;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::sort(ds.begin(), ds.end(), compareData);
    std::copy(
        ds.begin(),
        ds.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}