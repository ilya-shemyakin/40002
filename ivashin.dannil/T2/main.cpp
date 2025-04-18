#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cctype>

struct DataStruct {
    std::string key1;
    std::string key2;
    std::string key3;
};

class IofmtGuard {
public:
    IofmtGuard(std::basic_ios<char>& s) : s_(s), flags_(s.flags()) {}
    ~IofmtGuard() { s_.flags(flags_); }
private:
    std::basic_ios<char>& s_;
    std::ios_base::fmtflags flags_;
};

struct DelimiterIO { char exp; };
struct StringIO { std::string& ref; };
struct LabelIO { std::string exp; };

std::istream& operator>>(std::istream& in, DelimiterIO&& dest);
std::istream& operator>>(std::istream& in, StringIO&& dest);
std::istream& operator>>(std::istream& in, LabelIO&& dest);

bool parseComplex(std::istream& in, std::string& value) {
    std::string part;
    int depth = 1;
    char c;
    while (depth > 0 && in.get(c)) {
        part += c;
        if (c == '(') depth++;
        else if (c == ')') depth--;
    }
    if (depth != 0) return false;
    value = "#c" + part;
    return true;
}

std::istream& operator>>(std::istream& in, StringIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    std::string tmp;
    char c;
    if (in >> c && c == '"') {
        std::getline(in, tmp, '"');
        dest.ref = tmp;
    }
    else if (c == '#') {
        if (!in.get(c) || c != 'c') {
            in.setstate(std::ios::failbit);
            return in;
        }
        if (!parseComplex(in, dest.ref)) in.setstate(std::ios::failbit);
    }
    else if (c == '\'') {
        if (in.get(c) && c == '\'') dest.ref = "''";
        else {
            in.unget();
            dest.ref = "'";
        }
    }
    else {
        in.unget();
        in >> tmp;
        dest.ref = tmp;
    }
    return in;
}

std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char c = 0;
    if (in >> c && c != dest.exp) in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, LabelIO&& dest) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    std::string label;
    in >> StringIO{ label };
    if (label != dest.exp) in.setstate(std::ios::failbit);
    return in;
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    DataStruct tmp;
    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    in >> DelimiterIO{ '(' };
    while (true) {
        std::string key;
        in >> DelimiterIO{ ':' } >> StringIO{ key };
        if (key == "key1") {
            in >> StringIO{ tmp.key1 };
            hasKey1 = true;
        }
        else if (key == "key2") {
            in >> StringIO{ tmp.key2 };
            hasKey2 = true;
        }
        else if (key == "key3") {
            in >> StringIO{ tmp.key3 };
            hasKey3 = true;
        }
        else {
            in.setstate(std::ios::failbit);
            break;
        }
        char c;
        if (!in.get(c)) break;
        if (c == ')') {
            if (hasKey1 && hasKey2 && hasKey3) {
                data = tmp;
                return in;
            }
            in.setstate(std::ios::failbit);
            break;
        }
        in.unget();
    }
    in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    IofmtGuard guard(out);
    out << "(:key1 ";
    if (data.key1.find(' ') != std::string::npos || data.key1.empty()) {
        out << '"' << data.key1 << '"';
    }
    else {
        out << data.key1;
    }
    out << " :key2 ";
    if (data.key2.find(' ') != std::string::npos || data.key2.empty()) {
        out << '"' << data.key2 << '"';
    }
    else {
        out << data.key2;
    }
    out << " :key3 ";
    if (data.key3.find(' ') != std::string::npos || data.key3.empty()) {
        out << '"' << data.key3 << '"';
    }
    else {
        out << data.key3;
    }
    out << " :)";
    return out;
}

bool compareData(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

int main() {
    std::vector<DataStruct> data;
    bool hasValid = false;

    while (std::cin.good()) {
        DataStruct tmp;
        if (std::cin >> tmp) {
            data.push_back(tmp);
            hasValid = true;
        }
        else {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    if (hasValid) {
        std::cout << "At least one supported record type\n";
        std::sort(data.begin(), data.end(), compareData);
        for (const auto& elem : data) {
            std::cout << elem << "\n";
        }
    }
    else {
        std::cout << "No supported records found.\n";
    }

    return 0;
}
