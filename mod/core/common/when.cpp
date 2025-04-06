#include "when.hpp"
#include "../frame/thread.hpp"
#include "../builtin/err/nerr.hpp"
#include "../builtin/err/errReport.hpp"

namespace nm {

    NM(DEF_ME(__WHEN_OBJECT__))

    const me& me::get() {
        static me inner;
        return inner;
    }

    errReport& me::_getDefault() const {
        return thread::get() TO(getEx()) orRet dummyErrReport::singletone;
    }

    void me::_addNewErr(errReport& rpt, int code, ...) const {
        va_list args;
        va_start(args, code);
        rpt.add(nerr::newErr(code, args));
        va_end(args);
    }

    void me::_addNewErr(errReport& rpt, const point& src, int code, ...) const {
        va_list(args);
        va_start(args, code);
        rpt.add(nerr::newErr(src, code, args));
        va_end(args);
    }
}
