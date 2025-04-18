```cpp
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <iomanip>
#include <limits>

namespace educational {

    struct DataStruct {
        unsigned long long key1; // ULL LIT (e.g., 10ull)
        unsigned long long key2; // ULL HEX (e.g., 0xA)
        std::string key3;       
    };

    // Вспомогательные структуры для обработки ввода
    struct DelimiterIO { char exp; };
    struct ULLLitIO { unsigned long long& value; };
    struct ULLHexIO { unsigned long long& value; };
    struct StringIO { std::string& ref; };

    // Класс для сохранения состояния потока
    class iofmtguard {
    public:
        iofmtguard(std::basic_ios<char>& s)
            : s_(s), width_(s.width()), fill_(s.fill()), precision_(s.precision()), fmt_(s.flags()) {
        }
        ~iofmtguard() {
            s_.width(width_);
            s_.fill(fill_);
            s_.precision(precision_);
            s_.flags(fmt_);
        }
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    // Оператор ввода для разделителей
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        char c;
        in >> c;
        if (c != dest.exp) in.setstate(std::ios::failbit);
        return in;
    }

    // Оператор ввода для ULL LIT
    std::istream& operator>>(std::istream& in, ULLLitIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        std::string str;
        in >> str;
        if (str.size() < 3) {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::string suffix = str.substr(str.size() - 3);
        // Приводим суффикс к нижнему регистру для проверки
        for (char& c : suffix) c = std::tolower(c);
        if (suffix != "ull") {
            in.setstate(std::ios::failbit);
            return in;
        }
        try {
            dest.value = std::stoull(str.substr(0, str.size() - 3));
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    // Оператор ввода для ULL HEX
    std::istream& operator>>(std::istream& in, ULLHexIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        std::string str;
        in >> str;
        if (str.size() < 2 || (str.substr(0, 2) != "0x" && str.substr(0, 2) != "0X")) {
            in.setstate(std::ios::failbit);
            return in;
        }
        try {
            dest.value = std::stoull(str.substr(2), nullptr, 16);
        }
        catch (...) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    // Оператор ввода для строки
    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        char quote;
        in >> quote;
        if (quote != '"') {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::getline(in, dest.ref, '"');
        return in;
    }

    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct tmp;
        bool has_key1 = false, has_key2 = false, has_key3 = false;

        in >> DelimiterIO{ '(' } >> DelimiterIO{ ':' };

        while (true) {
            std::string key;
            in >> key;
            if (in.fail()) break;

            if (key == ":)") {
                break;
            }

            in >> DelimiterIO{ ' ' };

            if (key == "key1") {
                if (has_key1) {
                    in.setstate(std::ios::failbit);
                    break;
                }
                in >> ULLLitIO{ tmp.key1 };
                has_key1 = true;
            }
            else if (key == "key2") {
                if (has_key2) {
                    in.setstate(std::ios::failbit);
                    break;
                }
                in >> ULLHexIO{ tmp.key2 };
                has_key2 = true;
            }
            else if (key == "key3") {
                if (has_key3) {
                    in.setstate(std::ios::failbit);
                    break;
                }
                in >> StringIO{ tmp.key3 };
                has_key3 = true;
            }
            else {
                in.setstate(std::ios::failbit);
                break;
            }

            // Проверяем разделитель
            in >> DelimiterIO{ ':' };
            if (in.fail()) break;
        }

        // Проверяем, что все поля прочитаны
        if (in && has_key1 && has_key2 && has_key3) {
            dest = tmp;
        }
        else {
            in.setstate(std::ios::failbit);
        }

        // Пропускаем некорректные данные до следующей записи
        if (in.fail()) {
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '(');
        }

        return in;
    }

    std::ostream& operator<<(std::ostream& out, const DataStruct& src) {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;
        iofmtguard fmtguard(out);
        out << "(:key1 " << src.key1 << "ull:key2 0x" << std::hex << std::uppercase << src.key2
            << std::dec << ":key3 \"" << src.key3 << "\":)";
        return out;
    }

    // Функция сравнения для сортировки
    bool compare(const DataStruct& a, const DataStruct& b) {
        if (a.key1 != b.key1) return a.key1 < b.key1;
        if (a.key2 != b.key2) return a.key2 < b.key2;
        return a.key3.size() < b.key3.size();
    }

} // namespace educational

int main() {
    std::vector<educational::DataStruct> data;
    std::copy(std::istream_iterator<educational::DataStruct>(std::cin),
        std::istream_iterator<educational::DataStruct>(),
        std::back_inserter(data));

    std::sort(data.begin(), data.end(), educational::compare);

    std::copy(data.begin(), data.end(),
        std::ostream_iterator<educational::DataStruct>(std::cout, "\n"));

    return 0;
}
