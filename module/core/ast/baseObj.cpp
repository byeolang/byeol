#include "core/ast/baseObj.hpp"

#include "core/builtin/container/mgd/arr.hpp"
#include "core/builtin/container/tucontainable.inl"
#include "core/frame/thread.hpp"
#include "core/ast/args.hpp"
#include "core/ast/baseFunc.hpp"
#include "core/ast/node.inl"
#include "core/ast/src/dumSrc.hpp"

namespace by {

    BY(DEF_ME(baseObj))

    me::baseObj(const baseObj* org, /*UNUSED*/ nbool): _org(org) {}

    str me::run(const args& a) {
        tpriorities<baseFunc> matches = subAll<baseFunc>(ctor::CTOR_NAME, a);
        switch(matches.len()) {
            case 1: return _onBeforeCtor()->run(ctor::CTOR_NAME, a);
            case 0: return BY_W("there is no such ctor."), str();
        }
        /*// TODO: 1. change err management module to use 'err' class, not errCode.
          //       2. let it log all ambigious funcs here.
          return BY_W("")*/
        return BY_E("ambigious call found: %s", "TODO:"), str();
    }

    scope& me::subs() {
        const baseObj& org = getOrigin();
        return &org == this ? dumScope::singleton() : (scope&) org.subs();
    }

    state me::getState() const { return LINKED; }

    void me::setState(state new1) {}

    priorType me::prioritize(const args& a) const { return subAll<baseFunc>(ctor::CTOR_NAME, a).getPriorType(); }

    const baseObj& me::getOrigin() const { return _org ? *_org : *this; }

    void me::inFrame(const bicontainable* args) const {
        frames& frs = by::thread::get()._getFrames();
        BY_DI("%s.inFrame() frames.len[%d]", *this, frs.len());

        frame& fr = *new frame();
        frs.add(fr);
        _inFrame(fr, args);
    }

    void me::outFrame() const {
        frames& frs = by::thread::get()._getFrames();
        BY_DI("%s._outFrame() frames.len[%d]", *this, frs.len() - 1);

        frs.del();
    }

    const src& me::getSrc() const {
        WHEN(&getOrigin() == this) .ret(dumSrc::singleton());

        return getOrigin().getSrc();
    }

    const node& me::getSubPack() const {
        static obj dummy;
        WHEN(&getOrigin() == this)
            .ret(dummy); // which means, the derived origin class doesn't
                         // override getSrc(). to prevent infinite loop.
        return getOrigin().getSubPack();
    }

    baseObj* me::make() const { return (baseObj*) clone(); }

    const modifier& me::getModifier() const {
        WHEN(&getOrigin() == this) .ret(super::getModifier());
        return getOrigin().getModifier();
    }

    str me::_onRunSub(node& sub, const args& a) {
        a.setMe(*this);
        return super::_onRunSub(sub, a);
    }

    void me::_setSrc(const src& s) {}

    void me::_inFrame(frame& fr, const bicontainable* args) const {
        fr.setMe(*this);
        fr.add(*this);

        scope* s = new scope();
        s->add("me", *this);
        fr.add(*s);
    }

    void me::_setOrigin(const baseObj& newOrg) { _org.bind(newOrg); }

    void me::_setType(const mgdType& new1) {}

    void me::_setModifier(const modifier& mod) {}

    str me::_onBeforeCtor() { return getOrigin(); }
} // namespace by
