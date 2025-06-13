#ifndef MAX_IOFMTGUARD_H
#define MAX_IOFMTGUARD_H
#include <ios>
class iofmtguard
{
public:
    explicit iofmtguard(std::basic_ios<char>& s):s_(s),w_(s.width()),f_(s.fill()),p_(s.precision()),fl_(s.flags()){}
    ~iofmtguard(){s_.width(w_);s_.fill(f_);s_.precision(p_);s_.flags(fl_);}
private:
    std::basic_ios<char>&               s_;
    std::streamsize                     w_;
    char                                f_;
    std::streamsize                     p_;
    std::basic_ios<char>::fmtflags      fl_;
};
#endif
