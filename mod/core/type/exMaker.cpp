#include "exMaker.hpp"

#include "../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(exMaker))

    void me::make(nint code, ...) {
        errReport& rpt = thread::get().getEx();
        va_list args;
        va_start(args, code);

        err* new1 = err::newErr(code, args);
        new1->dbgLog();
        rpt.add(new1);

        va_end(args);
    }

    void me::make(const point& pos, int code, ...) {
        errReport& rpt = thread::get().getEx();
        va_list args;
        va_start(args, code);

        err* new1 = err::newErr(pos, code, args);
        new1->dbgLog();
        rpt.add(new1);

        va_end(args);
    }
}
