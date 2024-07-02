#include "exceptional.hpp"
#include "../frame/thread.hpp"

namespace nm {
    NAMU(DEF_ME(exceptional))

    void me::ex(nint code, ...) {
        errReport& rpt = thread::get().getEx();
        va_list args;
        va_start(args, code);
        rpt.add(err::newErr(code, args));
        va_end(args);
    }

    void me::ex(const point& pos, int code, ...) {
        errReport& rpt = thread::get().getEx();
        va_list args;
        va_start(args, code);
        rpt.add(err::newErr(pos, code, args));
        va_end(args);
    }
}
