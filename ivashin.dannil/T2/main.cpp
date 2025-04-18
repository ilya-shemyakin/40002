#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>
#include <cctype>
#include <iomanip>
#include <limits>

namespace nspace {

    struct DataStruct {
        unsigned long long key1 = 0; // ULL LIT
        unsigned long long key2 = 0; // ULL HEX
        std::string key3;
    };

    struct DelimiterIO { std::string exp; };
    struct ULLLitIO { unsigned long long& value; };
    struct ULLHexIO { unsigned long long& value; };
    struct StringIO { std::string& ref; };

    class iofmtguard {
    public:
        iofmtguard(std::basic_ios<char>& s);
        ~iofmtguard();
    private:
        std::basic_ios<char>& s_;
        std::streamsize width_;
        char fill_;
        std::streamsize precision_;
        std::basic_ios<char>::fmtflags fmt_;
    };

    iofmtguard::iofmtguard(std::basic_ios<char>& s) :
        s_(s), width_(s.width()), fill_(s.fill()),
        precision_(s.precision()), fmt_(s.flags()) {
    }

    iofmtguard::~iofmtguard() {
        s_.width(width_);
        s_.fill(fill_);
        s_.precision(precision_);
        s_.flags(fmt_);
    }

    // Чтение разделителей (строковых)
    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        std::string input;
        for (char c : dest.exp) {
            char got;
            in.get(got);
            input += got;
            if (!in || got != c) {
                in.setstate(std::ios::failbit);
                return in;
            }
        }
        return in;
    }

    // Чтение ULL LIT
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
        for (char& ch : suffix) ch = std::tolower(ch);
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

    // Чтение ULL HEX
    std::istream& operator>>(std::istream& in, ULLHexIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        std::string str;
        in >> str;
        if (str.size() < 2 || (str.substr(0, 2) != "0x" &&
            str.substr(0, 2) != "0X")) {
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

    // Чтение строки
    std::istream& operator>>(std::istream& in, StringIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;
        char quote;
        in >> std::noskipws >> quote; // Не пропускать пробелы перед кавычкой
        if (quote != '"') {
            in.setstate(std::ios::failbit);
            return in;
        }
        dest.ref.clear();
        char c;
        while (in >> std::noskipws >> c && c != '"') {
            dest.ref += c;
        }
        if (c != '"') {
            in.setstate(std::ios::failbit);
        }
        in >> std::skipws; // Восстанавливаем стандартное поведение
        return in;
    }

    // Чтение DataStruct
    std::istream& operator>>(std::istream& in, DataStruct& dest) {
        std::istream::sentry sentry(in);
        if (!sentry) return in;

        DataStruct tmp; // Поля уже инициализированы: key1=0, key2=0, key3=""
        bool hasKey1 = false, hasKey2 = false, hasKey3 = false;

        in >> DelimiterIO{ "(:" }; // Начальный разделитель
        if (in.fail()) {
            in.clear();
            in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return in;
        }

        while (in) {
            std::string key;
            in >> key;
            if (in.fail()) {
                in.setstate(std::ios::failbit);
                break;
            }

            if (key == ":)") {
                if (hasKey1 && hasKey2 && hasKey3) {
                    dest = tmp; // Присваиваем только при успешном парсинге всех ключей
                    return in;
                }
                else {
                    in.setstate(std::ios::failbit);
                    break;
                }
            }

            in >> DelimiterIO{ " " };
            if (in.fail()) break;

            if (key == "key1") {
                if (hasKey1) {
                    in.setstate(std::ios::failbit);
                    break;
                }
                in >> ULLLitIO{ tmp.key1 };
                hasKey1 = in.good();
            }
            else if (key == "key2") {
                if (hasKey2) {
                    in.setstate(std::ios::failbit);
                    break;
                }
                in >> ULLHexIO{ tmp.key2 };
                hasKey2 = in.good();
            }
            else if (key == "key3") {
                if (hasKey3) {
                    in.setstate(std::ios::failbit);
                    break];
                }
                in >> StringIO{ tmp.key3 };
                hasKey3 = in.good();
            }
            else {
                in.setstate(std::ios::failbit);
                break;
            }

            in >> DelimiterIO{ ":" };
            if (in.fail()) break;
        }

        // Если парсинг не завершился успешно, устанавливаем failbit
        in.setstate(std::ios::failbit);
        in.clear();
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return in;
    }

    // Вывод DataStruct
    std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
        std::ostream::sentry sentry(out);
        if (!sentry) return out;
        iofmtguard guard(out);
        out << "(:key1 " << data.key1 << "ull:key2 0x";
        out << std::uppercase << std::hex << data.key2;
        out << std::dec << ":key3 \"" << data.key3 << "\":)";
        return out;
    }

} // namespace nspace

bool compare(const nspace::DataStruct& a, const nspace::DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
}

int main() {
    std::vector<nspace::DataStruct> data;

    // Чтение данных
    std::copy(std::istream_iterator<nspace::DataStruct>(std::cin),
        std::istream_iterator<nspace::DataStruct>(),
        std::back_inserter(data));

    // Проверка на отсутствие валидных записей
    if (data.empty()) {
        std::cout << "Looks like there is no supported record. ";
        std::cout << "Cannot determine input. Test skipped\n";
        return 0;
    }

    // Сообщение о наличии записей
    std::cout << "Atleast one supported record type\n";

    // Сортировка
    std::sort(data.begin(), data.end(), compare);

    // Вывод результатов
    std::copy(data.begin(), data.end(),
        std::ostream_iterator<nspace::DataStruct>(std::cout, "\n"));

    return 0;
}
