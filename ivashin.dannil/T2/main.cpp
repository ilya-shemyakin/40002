#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct DataStruct {
    unsigned long long key1_;
    unsigned long long key2_;
    std::string key3_;
};

// Class to save and restore stream format
class IofmtGuard {
public:
    IofmtGuard(std::basic_ios<char>& stream) : stream_(stream), flags_(stream.flags()) {}
    ~IofmtGuard() { stream_.flags(flags_); }

private:
    std::basic_ios<char>& stream_;
    std::ios_base::fmtflags flags_;
};

// Input operator for DataStruct
std::istream& operator>>(std::istream& input, DataStruct& data) {
    std::istream::sentry sentry(input);
    if (!sentry) {
        return input;
    }

    DataStruct temp;
    std::vector<bool> keysPresent(3, false);
    char c;
    std::string token;

    // Expect opening parenthesis
    if (!(input >> c) || c != '(') {
        input.setstate(std::ios::failbit);
        return input;
    }

    bool valid = true;
    while (valid && input >> c && c == ':') {
        if (!(input >> token)) {
            valid = false;
            break;
        }

        if (token == "key1" && !keysPresent[0]) {
            std::string valueStr;
            if (!(input >> valueStr)) {
                valid = false;
                break;
            }
            // Check for ULL LIT format (e.g., "123ull" or "123ULL")
            if (valueStr.size() < 4 ||
                (valueStr.substr(valueStr.size() - 3) != "ull" && valueStr.substr(valueStr.size() - 3) != "ULL")) {
                valid = false;
                break;
            }
            std::istringstream iss(valueStr.substr(0, valueStr.size() - 3));
            unsigned long long value;
            if (!(iss >> value)) {
                valid = false;
                break;
            }
            temp.key1_ = value;
            keysPresent[0] = true;
        }
        else if (token == "key2" && !keysPresent[1]) {
            std::string hexString;
            if (!(input >> hexString)) {
                valid = false;
                break;
            }
            // Check for ULL HEX format (e.g., "0xFFA")
            if (hexString.size() < 3 || hexString.substr(0, 2) != "0x") {
                valid = false;
                break;
            }
            std::istringstream iss(hexString.substr(2));
            unsigned long long value;
            if (!(iss >> std::hex >> value)) {
                valid = false;
                break;
            }
            temp.key2_ = value;
            keysPresent[1] = true;
        }
        else if (token == "key3" && !keysPresent[2]) {
            input >> std::ws;
            if (input.get() != '"') {
                valid = false;
                break;
            }
            std::getline(input, temp.key3_, '"');
            keysPresent[2] = true;
        }
        else {
            valid = false;
            break;
        }
    }

    // Check for closing parenthesis and all keys present
    if (valid && input >> c && c == ')' && keysPresent[0] && keysPresent[1] && keysPresent[2]) {
        data = temp;
    }
    else {
        input.setstate(std::ios::failbit);
    }

    return input;
}

// Output operator for DataStruct
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

// Comparator for sorting
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
    std::vector<DataStruct> data;
    bool hasValidRecord = false;

    while (!std::cin.eof()) {
        std::istream::pos_type startPos = std::cin.tellg();
        DataStruct temp;
        std::cin >> temp;
        if (std::cin) {
            data.push_back(temp);
            hasValidRecord = true;
        }
        else {
            std::cin.clear();
            if (startPos != -1) {
                std::cin.seekg(startPos);
            }
            // Skip to the next line or closing parenthesis
            char c;
            while (std::cin.get(c) && c != '\n' && c != ')') {
                if (c == '(') {
                    // Consume until closing parenthesis
                    int parenCount = 1;
                    while (std::cin.get(c) && parenCount > 0) {
                        if (c == '(') parenCount++;
                        if (c == ')') parenCount--;
                    }
                    break;
                }
            }
            if (c == ')') {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    if (hasValidRecord) {
        std::cout << "Atleast one supported record type\n";
        std::sort(data.begin(), data.end(), compareData);
        std::copy(data.begin(), data.end(), std::ostream_iterator<DataStruct>(std::cout, "\n"));
    }
    else {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped\n";
    }

    return 0;
}
