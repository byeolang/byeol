#include "exMaker.hpp"

#include "../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(exMaker))

    void me::make(nint code, ...) {
        errReport& rpt = thread::get().getEx();
        va_list args;
        va_start(args, code);
        rpt.add(err::newErr(code, args));
        va_end(args);
    }

    void me::make(const point& pos, int code, ...) {
        errReport& rpt = thread::get().getEx();
        va_list args;
        va_start(args, code);
        rpt.add(err::newErr(pos, code, args));
        va_end(args);
    }
}
