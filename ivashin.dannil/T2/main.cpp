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

// Класс для сохранения и восстановления формата потока
class IofmtGuard {
public:
    IofmtGuard(std::basic_ios<char>& stream) : stream_(stream), flags_(stream.flags()) {}
    ~IofmtGuard() { stream_.flags(flags_); }
private:
    std::basic_ios<char>& stream_;
    std::ios_base::fmtflags flags_;
};

bool parseDataStruct(const std::string& line, DataStruct& data) {
    std::istringstream iss(line);
    char c;
    if (!(iss >> c) || c != '(') {
        return false;
    }

    DataStruct temp;
    bool keysPresent[3] = { false, false, false };

    while (iss.peek() == ':') {
        iss.ignore(1, ':'); // Пропускаем ':'
        if (iss.peek() == ')') {
            break; 
        }
        std::string token;
        if (!(iss >> token)) {
            return false;
        }
        if (token == "key1" && !keysPresent[0]) {
            std::string valueStr;
            if (!(iss >> valueStr)) {
                return false;
            }
            // Проверка формата ULL LIT
            if (valueStr.size() < 4 ||
                (valueStr.substr(valueStr.size() - 3) != "ull" && valueStr.substr(valueStr.size() - 3) != "ULL")) {
                return false;
            }
            std::istringstream numIss(valueStr.substr(0, valueStr.size() - 3));
            unsigned long long value;
            if (!(numIss >> value) || numIss.rdbuf()->in_avail() > 0) {
                return false;
            }
            temp.key1_ = value;
            keysPresent[0] = true;
        }
        else if (token == "key2" && !keysPresent[1]) {
            std::string hexString;
            if (!(iss >> hexString)) {
                return false;
            }
            // Проверка формата ULL HEX
            if (hexString.size() < 3 || hexString.substr(0, 2) != "0x") {
                return false;
            }
            std::istringstream numIss(hexString.substr(2));
            unsigned long long value;
            if (!(numIss >> std::hex >> value) || numIss.rdbuf()->in_avail() > 0) {
                return false;
            }
            temp.key2_ = value;
            keysPresent[1] = true;
        }
        else if (token == "key3" && !keysPresent[2]) {
            iss >> std::ws;
            if (iss.get() != '"') {
                return false;
            }
            std::getline(iss, temp.key3_, '"');
            if (iss.fail()) {
                return false;
            }
            keysPresent[2] = true;
        }
        else {
            return false;
        }
    }

    if (iss >> c && c == ')' && keysPresent[0] && keysPresent[1] && keysPresent[2]) {
        data = temp;
        return true;
    }
    return false;
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
    std::vector<DataStruct> data;
    bool hasValidRecord = false;
    std::string line;

    while (std::getline(std::cin, line)) {
        DataStruct temp;
        if (parseDataStruct(line, temp)) {
            data.push_back(temp);
            hasValidRecord = true;
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
