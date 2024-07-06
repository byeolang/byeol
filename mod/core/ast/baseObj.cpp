#include "../builtin/container/mgd/arr.hpp"
#include "baseObj.hpp"
#include "baseFunc.hpp"
#include "../builtin/container/tucontainable.inl"
#include "../frame/thread.hpp"
#include "args.hpp"
#include "baseFunc.hpp"
#include "node.inl"
#include "src/dumSrc.hpp"

namespace nm {

    NM(DEF_ME(baseObj))

    me::baseObj(): _src(dumSrc::singletone()) {}

    str me::_onRunSub(node& sub, const args& a) {
        a.setMe(*this);
        return super::_onRunSub(sub, a);
    }

    str me::run(const args& a) {
        return str(this);
    }

    priorType me::prioritize(const args& a) const {
        return subAll<baseFunc>(baseObj::CTOR_NAME, a).getPriorType();
    }

    void me::inFrame(const bicontainable& args) {
        frames& frs = nm::thread::get()._getFrames();
        NM_DI("%s.inFrame() frames.len[%s]", *this, frs.len());

        frame& fr = *new frame();
        frs.add(fr);
        _inFrame(fr, args);
    }

    void me::_inFrame(frame& fr, const bicontainable& args) {
        fr.setMe(*this);
        fr.add(*this);

        scope* s = new scope();
        s->add("me", *this);
        fr.add(*s);
    }

    void me::outFrame(const bicontainable& args) {
        frames& frs = nm::thread::get()._getFrames();
        NM_DI("%s._outFrame() frames.len[%s]", *this, frs.len()-1);

        frs.del();
    }

    const src& me::getSrc() const {
        return *_src;
    }

    nbool me::isPreEvaluated() const { return true; }

    void me::_setSrc(const src& s) {
        _src.bind(s);
    }
}
