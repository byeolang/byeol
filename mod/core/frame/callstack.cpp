#include "callstack.hpp"

#include "../ast/baseFunc.hpp"
#include "../ast/params.hpp"
#include "../ast/src/src.hpp"

namespace nm {

    calltrace::calltrace(const frame& newFrame) {
        fr.bind(newFrame);
        if(!fr) return;

        const baseFunc& f = fr->getFunc() OR_RET;
        const src& s = f.getSrc();

        at = s.getName() + "(" + f.getParams().toStr() + ")";
        const srcFile& file = s.getFile();
        if(!nul(file)) in = file.getFileName() + ":" + std::to_string(s.getPos().row);
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
