#include "DataStruct.h"
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>

namespace max {

class IOfmtGuard {
    std::ios& s_;
    std::ios::fmtflags f_;
    std::streamsize p_, w_;
    char fill_;
public:
    explicit IOfmtGuard(std::ios& s): s_(s), f_(s.flags()), p_(s.precision()), w_(s.width()), fill_(s.fill()) {}
    ~IOfmtGuard() { s_.flags(f_); s_.precision(p_); s_.width(w_); s_.fill(fill_); }
};

static std::vector<std::string> splitIgnoringQuotedColons(const std::string& line) {
    std::vector<std::string> parts;
    std::string chunk;
    bool inQuotes = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        char c = line[i];
        if (c == '"' && (i == 0 || line[i - 1] != '\\')) inQuotes = !inQuotes;
        if (c == ':' && !inQuotes) {
            parts.push_back(chunk);
            chunk.clear();
        } else {
            chunk += c;
        }
    }
    parts.push_back(chunk);
    return parts;
}

static bool parseLine(const std::string& raw, DataStruct& out) {
    auto parts = splitIgnoringQuotedColons(raw);
    parts.erase(std::remove_if(parts.begin(), parts.end(),
                               [](const std::string& s){ return s.empty(); }),
                parts.end());
    if (parts.size() != 5 || parts[0] != "(" || parts[4] != ")") return false;
    bool h1 = false, h2 = false, h3 = false;
    try {
        for (std::size_t i = 1; i <= 3; ++i) {
            std::istringstream field(parts[i]);
            std::string key; field >> key;
            if (key == "key1") {
                std::string val; field >> val;
                if (val.empty() || (val.back() != 'd' && val.back() != 'D')) return false;
                out.key1 = std::stod(val.substr(0, val.size() - 1));
                h1 = true;
            } else if (key == "key2") {
                std::string val; field >> val;
                if (val.size() < 4 ||
                   (val.substr(val.size() - 3) != "ull" &&
                    val.substr(val.size() - 3) != "ULL")) return false;
                out.key2 = std::stoull(val.substr(0, val.size() - 3));
                h2 = true;
            } else if (key == "key3") {
                auto f = parts[i].find('"');
                auto l = parts[i].rfind('"');
                if (f == std::string::npos || l == f) return false;
                out.key3 = parts[i].substr(f + 1, l - f - 1);
                h3 = true;
            } else {
                return false;
            }
        }
    } catch (...) {
        return false;
    }
    return h1 && h2 && h3;
}

std::istream& operator>>(std::istream& in, DataStruct& dst) {
    std::string line;
    while (std::getline(in, line)) {
        auto b = line.find_first_not_of(" \t\r\n");
        if (b == std::string::npos) continue;
        auto e = line.find_last_not_of(" \t\r\n");
        std::string trimmed = line.substr(b, e - b + 1);
        DataStruct tmp;
        if (parseLine(trimmed, tmp)) {
            dst = tmp;
            return in;
        }
    }
    return in;
}

std::ostream& operator<<(std::ostream& os, DataStruct const& v) {
    std::ostream::sentry s(os);
    if (!s) return os;
    IOfmtGuard g(os);
    os << "(:key1 "
       << std::fixed << std::setprecision(1) << v.key1 << 'd'
       << ":key2 " << v.key2 << "ull"
       << ":key3 \"" << v.key3 << "\":)";
    return os;
}

bool cmpDataStruct(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
}

}
