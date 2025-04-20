#include "core/common/when.hpp"
#include "core/frame/thread.hpp"
#include "core/builtin/err/nerr.hpp"
#include "core/builtin/err/errReport.hpp"

namespace nm {

    NM(DEF_ME(__WHEN_OBJECT__))

    const me& me::get() {
        static me inner;
        return inner;
    }

    errReport& me::_getDefault() const {
        return thread::get() TO(getEx()) OR.ret(dummyErrReport::singleton);
    }

    const point& me::_getPosFrom(const node& src) const { return src.getSrc().getPos(); }

    void me::_addNewErr(errLv::level lv, int code, errReport* rpt, ...) const {
        va_list args;
        va_start(args, rpt);
        rpt->add(new nerr(lv, code, args));
        va_end(args);
    }

    void me::_addNewErr(errLv::level lv, const point& src, int code, errReport* rpt, ...) const {
        va_list args;
        va_start(args, rpt);
        rpt->add(new nerr(lv, src, code, args));
        va_end(args);
    }
}
