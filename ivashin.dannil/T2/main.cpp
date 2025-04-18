#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <limits>
#include <cmath>
#include <cctype>

struct DataStruct {
    std::string key1;
    std::string key2;
    std::string key3;
};

class IofmtGuard {
public:
    IofmtGuard(std::basic_ios<char>& stream) : stream_(stream), flags_(stream.flags()) {}
    ~IofmtGuard() { stream_.flags(flags_); }

private:
    std::basic_ios<char>& stream_;
    std::ios_base::fmtflags flags_;
};

bool parseValue(std::istream& input, std::string& value) {
    char c;
    input >> c;

    if (c == '"') {
        std::getline(input, value, '"');
        return true;
    }
    else if (c == '#') {
        if (input.get() != 'c' || input.get() != '(') return false;
        value = "#c(";
        std::string part;
        while (std::getline(input, part, ')')) {
            value += part;
            if (!part.empty() && part.back() == ')') break;
        }
        value += ")";
        return true;
    }
    else if (c == '(') {
        value = "(";
        int depth = 1;
        while (depth > 0 && input.get(c)) {
            value += c;
            if (c == '(') depth++;
            if (c == ')') depth--;
        }
        return depth == 0;
    }
    else if (c == '\'') {
        value = "'";
        if (input.get(c)) {
            value += c;
            if (input.get(c) && c == '\'') {
                value += "'";
                return true;
            }
        }
        return false;
    }
    else {
        input.unget();
        std::string temp;
        input >> temp;
        value = temp;
        return !temp.empty();
    }
}

std::istream& operator>>(std::istream& input, DataStruct& data) {
    DataStruct temp;
    char c;

    if (!(input >> c) || c != '(') {
        input.setstate(std::ios::failbit);
        return input;
    }

    bool keys[3] = { false };
    std::string key;

    while (input >> c && c == ':') {
        std::string field;
        input >> field;

        if (field == "key1" && !keys[0]) {
            if (!parseValue(input, temp.key1)) {
                input.setstate(std::ios::failbit);
                return input;
            }
            keys[0] = true;
        }
        else if (field == "key2" && !keys[1]) {
            if (!parseValue(input, temp.key2)) {
                input.setstate(std::ios::failbit);
                return input;
            }
            keys[1] = true;
        }
        else if (field == "key3" && !keys[2]) {
            if (!parseValue(input, temp.key3)) {
                input.setstate(std::ios::failbit);
                return input;
            }
            keys[2] = true;     
        }
        else {
            input.setstate(std::ios::failbit);
            return input;
        }
    }

    if (c == ')' && keys[0] && keys[1] && keys[2]) {
        data = temp;
        return input;
    }

    input.setstate(std::ios::failbit);
    return input;
}

std::ostream& operator<<(std::ostream& output, const DataStruct& data) {
    output << "(:key1 " << data.key1
        << ":key2 " << data.key2
        << ":key3 \"" << data.key3 << "\":)";
    return output;
}

bool compareData(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

int main() {
    std::vector<DataStruct> data;
    bool hasValid = false;

    while (true) {
        DataStruct temp;
        if (std::cin >> temp) {
            data.push_back(temp);
            hasValid = true;
        }
        else {
            if (std::cin.eof()) break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    if (hasValid) {
        std::cout << "Atleast one supported record type\n";
        std::sort(data.begin(), data.end(), compareData);
        for (const auto& item : data) {
            std::cout << item << "\n";
        }
    }
    else {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    }

    return 0;
}
