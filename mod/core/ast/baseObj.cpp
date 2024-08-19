#include "baseObj.hpp"

#include "../builtin/container/mgd/arr.hpp"
#include "../builtin/container/tucontainable.inl"
#include "../frame/thread.hpp"
#include "args.hpp"
#include "baseFunc.hpp"
#include "node.inl"
#include "src/dumSrc.hpp"

namespace nm {

    NM(DEF_ME(baseObj))

    str me::run(const args& a) {
        tpriorities<baseFunc> matches = subAll<baseFunc>(baseObj::CTOR_NAME, a);
        switch(matches.len()) {
            case 1: return run(baseObj::CTOR_NAME, a);
            case 0: return NM_W("there is no such ctor."), str();
        }
        /*// TODO: 1. change err management module to use 'err' class, not errCode.
          //       2. let it log all ambigious funcs here.
          return NM_W("")*/
        return NM_E("ambigious call found: %s", "TODO:"), str();
    }

    state me::getState() const { return LINKED; }

    void me::setState(state new1) {}

    priorType me::prioritize(const args& a) const {
        return subAll<baseFunc>(baseObj::CTOR_NAME, a).getPriorType();
    }

    void me::inFrame(const bicontainable& args) {
        frames& frs = nm::thread::get()._getFrames();
        NM_DI("%s.inFrame() frames.len[%d]", *this, frs.len());

        frame& fr = *new frame();
        frs.add(fr);
        _inFrame(fr, args);
    }

    void me::outFrame(const bicontainable& args) {
        frames& frs = nm::thread::get()._getFrames();
        NM_DI("%s._outFrame() frames.len[%d]", *this, frs.len() - 1);

        frs.del();
    }

    const src& me::getSrc() const {
        if(&getOrigin() == this) // which means, the derived origin class doesn't override getSrc().
            return dumSrc::singletone(); // to prevent infinite loop.
        return getOrigin().getSrc();
    }

    nbool me::isPreEvaluated() const { return true; }

    const obj& me::getSubPack() const {
        if(&getOrigin() == this) // which means, the derived origin class doesn't override getSrc().
            return nulOf<obj>(); // to prevent infinite loop.
        return getOrigin().getSubPack();
    }

    baseObj* me::make() const { return (baseObj*) clone(); }

    str me::_onRunSub(node& sub, const args& a) {
        a.setMe(*this);
        return super::_onRunSub(sub, a);
    }

    void me::_setSrc(const src& s) {}

    void me::_inFrame(frame& fr, const bicontainable& args) {
        fr.setMe(*this);
        fr.add(*this);

        scope* s = new scope();
        s->add("me", *this);
        fr.add(*s);
    }
} // namespace nm
