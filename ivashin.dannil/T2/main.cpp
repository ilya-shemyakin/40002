#include <algorithm>
#include <charconv>
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
    unsigned long long key1;
    unsigned long long key2;
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

bool parseNumber(std::string str, unsigned long long& result) {
    // Удаляем суффиксы
    if (str.size() > 2) {
        const std::string suffix = str.substr(str.size() - 3);
        if (suffix == "ull" || suffix == "ULL") {
            str = str.substr(0, str.size() - 3);
        }
    }

    if (str.size() > 1) {
        const std::string suffix = str.substr(str.size() - 2);
        if (suffix == "ll" || suffix == "LL") {
            str = str.substr(0, str.size() - 2);
        }
        else if (str.back() == 'd' || str.back() == 'D') {
            str = str.substr(0, str.size() - 1);
        }
    }

    // Парсим разные системы счисления
    int base = 10;
    if (str.size() > 1 && str[0] == '0') {
        if (str[1] == 'x' || str[1] == 'X') {
            base = 16;
            str = str.substr(2);
        }
        else if (str[1] == 'b' || str[1] == 'B') {
            base = 2;
            str = str.substr(2);
        }
        else if (isdigit(str[1])) {
            base = 8;
            str = str.substr(1);
        }
    }

    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result, base);
    return ec == std::errc() && ptr == str.data() + str.size();
}

std::istream& operator>>(std::istream& input, DataStruct& data) {
    DataStruct temp;
    char c;
    std::string token;

    if (!(input >> std::ws >> c) || c != '(') {
        input.setstate(std::ios::failbit);
        return input;
    }

    bool keys[3] = { false };
    bool valid = true;

    while (valid && input >> std::ws >> c && c == ':') {
        if (!(input >> token)) {
            valid = false;
            break;
        }

        if (token == "key1" && !keys[0]) {
            std::string value;
            if (input >> value) {
                if (!parseNumber(value, temp.key1)) {
                    valid = false;
                }
                else {
                    keys[0] = true;
                }
            }
        }
        else if (token == "key2" && !keys[1]) {
            std::string value;
            if (input >> value) {
                if (!parseNumber(value, temp.key2)) {
                    valid = false;
                }
                else {
                    keys[1] = true;
                }
            }
        }
        else if (token == "key3" && !keys[2]) {
            input >> std::ws;
            if (input.get() != '"') {
                valid = false;
                break;
            }
            std::getline(input, temp.key3, '"');
            keys[2] = true;
        }
        else {
            valid = false;
            break;
        }
    }

    if (valid && keys[0] && keys[1] && keys[2] &&
        input && c == ')' && (input >> std::ws).eof()) {
        data = temp;
    }
    else {
        input.setstate(std::ios::failbit);
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, const DataStruct& data) {
    IofmtGuard guard(output);
    output << "(:key1 " << data.key1 << "ull"
        << ":key2 0x" << std::hex << std::uppercase << data.key2
        << std::dec << ":key3 \"" << data.key3 << "\":)";
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
        std::copy(data.begin(), data.end(),
            std::ostream_iterator<DataStruct>(std::cout, "\n"));
    }
    else {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    }

    return 0;
}
