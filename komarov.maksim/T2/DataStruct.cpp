#include "DataStruct.h"
#include <iomanip>
#include <limits>
#include <cctype>

namespace max
{
  std::istream& operator>>(std::istream& in, DelimiterIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    char c = 0;
    in >> c;
    if (in && c != d.exp) in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, DoubleLiteralIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    in >> d.ref >> DelimiterIO{ 'd' };
    return in;
  }

  std::istream& operator>>(std::istream& in, UnsignedLongLongLiteralIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    in >> d.ref;
    char u = 0, l1 = 0, l2 = 0;
    in >> u >> l1 >> l2;
    bool ok = (u=='u'||u=='U')&&(l1=='l'||l1=='L')&&(l2=='l'||l2=='L');
    if (in && !ok) in.setstate(std::ios::failbit);
    return in;
  }

  std::istream& operator>>(std::istream& in, StringIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    return std::getline(in >> DelimiterIO{ '"' }, d.ref, '"');
  }

  std::istream& operator>>(std::istream& in, LabelIO&& d)
  {
    std::istream::sentry s(in);
    if (!s) return in;
    std::string word;
    in >> word;
    if (in && word != d.exp) in.setstate(std::ios::failbit);
    return in;
  }

  static void skip_ws_and_comments(std::istream& in)
  {
    for (;;)
    {
      in >> std::ws;
      if (in.peek() == '#')
      {
        in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
      }
      return;
    }
  }

  std::istream& operator>>(std::istream& in, DataStruct& dst)
  {
    skip_ws_and_comments(in);
    std::istream::sentry s(in);
    if (!s) return in;

    DataStruct tmp;
    bool k1 = false, k2 = false, k3 = false;

    in >> DelimiterIO{'('};

    for (;;)
    {
      in >> DelimiterIO{':'};
      if (in.peek() == ')') break;

      std::string key;
      in >> key;

      if (key == "key1")      { in >> DoubleLiteralIO{tmp.key1}; k1 = true; }
      else if (key == "key2") { in >> UnsignedLongLongLiteralIO{tmp.key2}; k2 = true; }
      else if (key == "key3") { in >> StringIO{tmp.key3};        k3 = true; }
      else                    { in.setstate(std::ios::failbit);  break;    }
    }

    in >> DelimiterIO{')'};

    if (in && k1 && k2 && k3) dst = tmp;
    else in.setstate(std::ios::failbit);
    return in;
  }

  IOFmtGuard::IOFmtGuard(std::basic_ios<char>& s)
    : stream_(s), w_(s.width()), p_(s.precision()), fill_(s.fill()), f_(s.flags()) {}

  IOFmtGuard::~IOFmtGuard()
  {
    stream_.width(w_);
    stream_.precision(p_);
    stream_.fill(fill_);
    stream_.flags(f_);
  }

  std::ostream& operator<<(std::ostream& out, const DataStruct& src)
  {
    std::ostream::sentry s(out);
    if (!s) return out;
    IOFmtGuard g(out);
    out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << 'd'
        << ":key2 " << src.key2 << "ull"
        << ":key3 \"" << src.key3 << "\":)";
    return out;
  }
}


