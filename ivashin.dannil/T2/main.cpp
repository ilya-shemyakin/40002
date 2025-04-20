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
    IofmtGuard(std::basic_ios<char>& stream) : stream_(stream), flags_(stream.flags()) {}
    ~IofmtGuard() { stream_.flags(flags_); }

private:
    std::basic_ios<char>& stream_;
    std::ios_base::fmtflags flags_;
};

std::istream& operator>>(std::istream& input, DataStruct& data) {
    std::istream::sentry sentry(input);
    if (!sentry) {
        return input;
    }

    DataStruct temp;
    char c;
    std::string token;

    if (!(input >> c) || c != '(') {
        input.setstate(std::ios::failbit);
        return input;
    }

    bool valid = true;
    bool keysPresent[3] = { false, false, false };

    while (valid && input >> c && c == ':') {
        if (!(input >> token)) {
            valid = false;
            break;
        }

        if (token == "key1" && !keysPresent[0]) {
            std::string valueStr;
            char nextChar;
            while (input.get(nextChar)) {
                if (nextChar == ':' || nextChar == ')') {
                    input.putback(nextChar);
                    break;
                }
                valueStr += nextChar;
            }

            if (valueStr.size() < 3 ||
                (valueStr.substr(valueStr.size() - 3) != "ull" &&
                    valueStr.substr(valueStr.size() - 3) != "ULL")) {
                valid = false;
                break;
            }

            std::istringstream iss(valueStr.substr(0, valueStr.size() - 3));
            unsigned long long value;
            if (!(iss >> value) || iss.rdbuf()->in_avail() > 0) {
                valid = false;
                break;
            }
            temp.key1_ = value;
            keysPresent[0] = true;
        }
        else if (token == "key2" && !keysPresent[1]) {
            if (!(input >> c) || c != '0') {
                valid = false;
                break;
            }
            if (!(input >> c) || (c != 'x' && c != 'X')) {
                valid = false;
                break;
            }

            unsigned long long value;
            if (!(input >> std::hex >> value)) {
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
            if (input.fail()) {
                valid = false;
                break;
            }
            keysPresent[2] = true;
        }
        else {
            if (token != ")") {
                valid = false;
            }
            break;
        }
    }

    if (valid && keysPresent[0] && keysPresent[1] && keysPresent[2]) {
        data = temp;
    }

    if (!valid) {
        input.setstate(std::ios::failbit);
    }

    return input;
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

    //----Test----

    /*DataStruct d;
    std::cin >> d;
    std::cout << d;*/

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
