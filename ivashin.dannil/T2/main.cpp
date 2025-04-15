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
    char c;
    if (!(iss >> c) || c != '(') return is.setstate(std::ios::failbit), is;

    unsigned long long k1 = 0, k2 = 0;
    std::string k3;
    bool has_k1 = false, has_k2 = false, has_k3 = false;
    std::string token;

    while (iss >> c && c == ':') {
        if (!(iss >> token)) break;
        if (token == "key1") {
            if (!(iss >> k1)) break;
            std::string suffix;
            iss >> suffix;
            if (suffix != "ull" && suffix != "ULL") break;
            has_k1 = true;
        }
        else if (token == "key2") {
            if (!(iss >> std::hex >> k2)) break;
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
        if (!(iss >> c) || c != ':') break;
    }

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

    std::sort(data.begin(), data.end(), [](const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.length() < b.key3.length();
        });

    std::copy(data.begin(), data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n"));

    std::cout << std::endl;
    return 0;
}