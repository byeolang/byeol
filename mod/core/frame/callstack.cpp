#include "callstack.hpp"
#include "../builtin/container/native/tnarr.inl"
#include "../builtin/container/native/tnchain.inl"
#include "../ast/scopes.hpp"
#include "../ast/src/src.hpp"
#include "../ast/baseFunc.hpp"
#include "../ast/params.hpp"

namespace namu {

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

    NAMU(DEF_ME(callstack))

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

    void me::dump() const {
        for(const auto& c : getTraces())
            NAMU_E("\tat %s in %s", c.at.c_str(), c.in.c_str());
    }

    void me::setStack(const frames& stack) {
        _stacks.rel();
        _stacks.add(stack);
        _traces.rel();
    }
}
