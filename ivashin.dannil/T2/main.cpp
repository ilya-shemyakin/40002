#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct DataStruct {
    unsigned long long key1;
    unsigned long long key2;
    std::string key3;
};

std::istream& operator>>(std::istream& is, DataStruct& ds) {
    std::string line;
    if (!std::getline(is, line)) {
        is.setstate(std::ios::failbit);
        return is;
    }

    std::istringstream iss(line);
    iss >> std::ws; // Пропускаем начальные пробелы
    char c;
    if (!(iss >> c) || c != '(') {
        is.setstate(std::ios::failbit);
        return is;
    }

    unsigned long long k1 = 0, k2 = 0;
    std::string k3;
    bool has_k1 = false, has_k2 = false, has_k3 = false;

    while (iss >> c && c == ':') {
        std::string token;
        if (!(iss >> token)) break;
        if (token == "key1") {
            if (!(iss >> k1)) break;
            std::string suffix;
            if (!(iss >> suffix) || (suffix != "ull" && suffix != "ULL")) break;
            has_k1 = true;
        }
        else if (token == "key2") {
            iss >> std::ws;
            std::string hex_val;
            if (!(iss >> hex_val)) break;
            if (hex_val.substr(0, 2) != "0x" && hex_val.substr(0, 2) != "0X") break;
            std::istringstream hex_stream(hex_val.substr(2));
            if (!(hex_stream >> std::hex >> k2)) break;
            has_k2 = true;
        }
        else if (token == "key3") {
            iss >> std::ws;
            if (iss.get() != '"') break;
            std::getline(iss, k3, '"');
            has_k3 = true;
        }
        else {
            break;
        }
        iss >> std::ws;
        if (!(iss >> c) || c != ':') break;
    }

    iss >> std::ws;
    if (has_k1 && has_k2 && has_k3 && iss >> c && c == ')') {
        ds.key1 = k1;
        ds.key2 = k2;
        ds.key3 = k3;
    }
    else {
        is.setstate(std::ios::failbit);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
    os << "(:key1 " << ds.key1 << "ull:key2 0x" << std::hex << std::uppercase
        << ds.key2 << std::dec << ":key3 \"" << ds.key3 << "\":)";
    return os;
}

int main() {
    std::vector<DataStruct> data;

    std::copy(std::istream_iterator<DataStruct>(std::cin),
        std::istream_iterator<DataStruct>(),
        std::back_inserter(data));

    if (data.empty()) {
        std::cout << "Looks like there is no supported record. "
            << "Cannot determine input. Test skipped\n";
    }
    else {
        std::cout << "Atleast one supported record type\n";
        std::sort(data.begin(), data.end(), [](const DataStruct& a, const DataStruct& b) {
            if (a.key1 != b.key1) return a.key1 < b.key1;
            if (a.key2 != b.key2) return a.key2 < b.key2;
            return a.key3.length() < b.key3.length();
            });

        std::copy(data.begin(), data.end(),
            std::ostream_iterator<DataStruct>(std::cout, "\n"));
    }

    std::cout << std::endl;
    return 0;
}