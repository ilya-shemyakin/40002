#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

namespace nspace {

    struct DataStruct {
        unsigned long long key1;
        unsigned long long key2;
        std::string key3;
    };

    class iofmtguard {
    public:
        iofmtguard(std::basic_ios<char>& s) : s_(s), fmt_(s.flags()) {}
        ~iofmtguard() { s_.flags(fmt_); }
    private:
        std::basic_ios<char>& s_;
        std::ios_base::fmtflags fmt_;
    };

    // Оператор ввода
    std::istream& operator>>(std::istream& is, DataStruct& ds) {
        std::istream::sentry sentry(is);
        if (!sentry) return is;

        DataStruct temp;
        std::vector<bool> keys(3, false); // Отслеживаем наличие key1, key2, key3
        char c;
        std::string token;

        if (!(is >> c) || c != '(') {
            is.setstate(std::ios::failbit);
            return is;
        }

        while (is >> c && c == ':') {
            if (!(is >> token)) {
                is.setstate(std::ios::failbit);
                break;
            }

            if (token == "key1" && !keys[0]) {
                unsigned long long val;
                std::string suffix;
                if (!(is >> val >> suffix) || (suffix != "ull" && suffix != "ULL")) {
                    is.setstate(std::ios::failbit);
                    break;
                }
                temp.key1 = val;
                keys[0] = true;
            }
            else if (token == "key2" && !keys[1]) {
                std::string hex_str;
                if (!(is >> hex_str) || hex_str.size() < 3 || hex_str.substr(0, 2) != "0x") {
                    is.setstate(std::ios::failbit);
                    break;
                }
                std::istringstream iss(hex_str.substr(2));
                unsigned long long val;
                if (!(iss >> std::hex >> val)) {
                    is.setstate(std::ios::failbit);
                    break;
                }
                temp.key2 = val;
                keys[1] = true;
            }
            else if (token == "key3" && !keys[2]) {
                is >> std::ws;
                if (is.get() != '"') {
                    is.setstate(std::ios::failbit);
                    break;
                }
                std::getline(is, temp.key3, '"');
                keys[2] = true;
            }
            else {
                is.setstate(std::ios::failbit);
                break;
            }
        }

        if (is && is >> c && c == ')' && keys[0] && keys[1] && keys[2]) {
            ds = temp;
        }
        else {
            is.setstate(std::ios::failbit);
        }

        return is;
    }

    std::ostream& operator<<(std::ostream& os, const DataStruct& ds) {
        std::ostream::sentry sentry(os);
        if (!sentry) return os;

        iofmtguard fmtguard(os);
        os << "(:key1 " << ds.key1 << "ull:key2 0x" << std::hex << std::uppercase
            << ds.key2 << std::dec << ":key3 \"" << ds.key3 << "\":)";
        return os;
    }

} 

using nspace::DataStruct;

bool cmp(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.length() < b.key3.length();
}

int main() {
    std::vector<DataStruct> data;

    // We read the data ignoring incorrect lines
    while (!std::cin.eof()) {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );
        if (!std::cin) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    // sort
    std::sort(data.begin(), data.end(), cmp);

    // output
    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}