#include "DataStructure.h"

namespace nspace {

    bool comparator(const DataStructure& struct1, const DataStructure& struct2) {
        if (struct1.key1 == struct2.key1 && std::abs(struct1.key2) == std::abs(struct2.key2))
            return struct1.key3.length() < struct2.key3.length();
        if (struct1.key1 == struct2.key1) {
            return std::abs(struct1.key2) < std::abs(struct2.key2);
        }
        return struct1.key1 < struct2.key1;
    }

    std::istream& operator>>(std::istream& in, DelimiterIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
            return in;

        char c = '0';
        in >> c;
        if (in && (c != dest.exp)) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream& operator>>(std::istream& in, OctalUnsignedLongLongIO&& dest) {
        std::istream::sentry sentry(in);
        if (!sentry)
        {
            return in;
        }
        return in >> DelimiterIO{ '0' } >> std::oct >> dest.ref;
    }

}