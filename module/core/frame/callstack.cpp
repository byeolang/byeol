#include "core/frame/callstack.hpp"

#include "core/ast/baseFunc.hpp"
#include "core/ast/params.hpp"
#include "core/ast/src/src.hpp"

namespace nm {

    calltrace::calltrace(const frame& newFrame) {
        fr.bind(newFrame);
        if(!fr) return;

        const baseFunc& f = fr->getFunc() OR_RET_CTOR;
        const src& s = f.getSrc();

        at = s.getName() + "(" + f.getParams().toStr() + ")";
        const srcFile& file = s.getFile() OR_RET_CTOR;
        in = file.getFileName() + ":" + std::to_string(s.getPos().row);
    }

    NM(DEF_ME(callstack))

    me::callstack() {}

    me::callstack(const frames& frs) {
        for(nidx n = frs.len() - 1; n >= 0; n--)
            add(new calltrace(frs[n]));
    }

    void me::dump() const {
        using platformAPI::foreColor;
        auto& log = logger::get();
        for(const auto& c: *this) {
            log.logFormatBypass("\tat %s%s %sin %s%s%s\n", foreColor(YELLOW).c_str(), c.at.c_str(),
                foreColor(LIGHTGRAY).c_str(), foreColor(GREEN).c_str(), c.in.c_str(),
                foreColor(LIGHTGRAY).c_str());
        }
    }
} // namespace nm
