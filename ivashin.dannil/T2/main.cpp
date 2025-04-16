#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

struct DataStruct
{
    unsigned long long key1_;
    unsigned long long key2_;
    std::string key3_;
};

class IofmtGuard
{
public:
    IofmtGuard(std::basic_ios<char>& stream)
        : stream_(stream)
        , flags_(stream.flags())
    {
    }

    ~IofmtGuard()
    {
        stream_.flags(flags_);
    }

private:
    std::basic_ios<char>& stream_;
    std::ios_base::fmtflags flags_;
};

std::istream& operator>>(std::istream& input, DataStruct& data)
{
    std::istream::sentry sentry(input);
    if (!sentry)
    {
        return input;
    }

    DataStruct temp;
    std::vector<bool> keysPresent(3, false);
    char c;
    std::string token;

    if (!(input >> c) || c != '(')
    {
        input.setstate(std::ios::failbit);
        return input;
    }

    while (input >> c && c == ':')
    {
        if (!(input >> token))
        {
            input.setstate(std::ios::failbit);
            break;
        }

        if (token == "key1" && !keysPresent[0])
        {
            unsigned long long value;
            std::string suffix;
            if (!(input >> value >> suffix) || (suffix != "ull" && suffix != "ULL"))
            {
                input.setstate(std::ios::failbit);
                break;
            }
            temp.key1_ = value;
            keysPresent[0] = true;
        }
        else if (token == "key2" && !keysPresent[1])
        {
            std::string hexString;
            if (!(input >> hexString) || hexString.size() < 3 || hexString.substr(0, 2) != "0x")
            {
                input.setstate(std::ios::failbit);
                break;
            }
            std::istringstream iss(hexString.substr(2));
            unsigned long long value;
            if (!(iss >> std::hex >> value))
            {
                input.setstate(std::ios::failbit);
                break;
            }
            temp.key2_ = value;
            keysPresent[1] = true;
        }
        else if (token == "key3" && !keysPresent[2])
        {
            input >> std::ws;
            if (input.get() != '"')
            {
                input.setstate(std::ios::failbit);
                break;
            }
            std::getline(input, temp.key3_, '"');
            keysPresent[2] = true;
        }
        else
        {
            input.setstate(std::ios::failbit);
            break;
        }
    }

    if (input && input >> c && c == ')' && keysPresent[0] && keysPresent[1] && keysPresent[2])
    {
        data = temp;
    }
    else
    {
        input.setstate(std::ios::failbit);
    }

    return input;
}

std::ostream& operator<<(std::ostream& output, const DataStruct& data)
{
    std::ostream::sentry sentry(output);
    if (!sentry)
    {
        return output;
    }

    IofmtGuard guard(output);
    output << "(:key1 " << data.key1_ << "ull:key2 0x" << std::hex << std::uppercase
        << data.key2_ << std::dec << ":key3 \"" << data.key3_ << "\":)";
    return output;
}

bool compareData(const DataStruct& first, const DataStruct& second)
{
    if (first.key1_ != second.key1_)
    {
        return first.key1_ < second.key1_;
    }
    if (first.key2_ != second.key2_)
    {
        return first.key2_ < second.key2_;
    }
    return first.key3_.length() < second.key3_.length();
}

int main()
{
    std::vector<DataStruct> data;

    while (!std::cin.eof())
    {
        std::copy(
            std::istream_iterator<DataStruct>(std::cin),
            std::istream_iterator<DataStruct>(),
            std::back_inserter(data)
        );
        if (!std::cin)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::sort(data.begin(), data.end(), compareData);

    std::copy(
        data.begin(),
        data.end(),
        std::ostream_iterator<DataStruct>(std::cout, "\n")
    );

    return 0;
}