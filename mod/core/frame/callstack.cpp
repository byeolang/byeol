#include "callstack.hpp"
#include "../builtin/container/native/tnarr.inl"
#include "../builtin/container/native/tnchain.inl"
#include "../ast/scope.hpp"
#include "../ast/src/src.hpp"
#include "../ast/baseFunc.hpp"
#include "../ast/params.hpp"

namespace nm {

    calltrace::calltrace(const frame& newFrame) {
        fr.bind(newFrame);
        if(!fr) return;
        const baseFunc& f = fr->getFunc();
        if(nul(f)) return;

        const src& s = f.getSrc();
        at = s.getName() + "(" + f.getParams().toStr() + ")";
        const srcFile& file = s.getFile();
        if(!nul(file))
            in = file.getFileName() + ":" + std::to_string(s.getPos().row);
    }

    NM(DEF_ME(callstack))

    me::callstack() {}

    tucontainable<frame>::iter me::begin() const {
        return _stacks.begin();
    }

    tucontainable<frame>::iter me::end() const {
        return _stacks.end();
    }

    const calltraces& me::getTraces() const {
        if(!_traces) {
            _traces.bind(new calltraces());
            for(const auto& fr : *this)
                _traces->add(new calltrace(fr));
        }
        return *_traces;
    }

    nbool me::hasTraces() const {
        return _stacks.len() > 0;
    }

    void me::dump() const {
        using platformAPI::foreColor;
        auto& log = logger::get();
        for(const auto& c : getTraces()) {
            log.logFormatBypass("\tat %s%s %sin %s%s%s\n", foreColor(YELLOW).c_str(), c.at.c_str(), foreColor(LIGHTGRAY).c_str(),
                                foreColor(GREEN).c_str(), c.in.c_str(), foreColor(LIGHTGRAY).c_str());
        }
    }

    void me::setStack(const frames& stack) {
        _stacks.rel();
        _stacks.add(stack);
        _traces.rel();
    }
}
