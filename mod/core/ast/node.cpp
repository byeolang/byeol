#include "core/ast/node.inl"

#include "core/builtin/container/native/tnarr.hpp"
#include "core/type/as.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/args.hpp"
#include "core/ast/modifier/dumModifier.hpp"
#include "core/ast/src/dumSrc.hpp"

namespace nm {


    NM(DEF_ME(node), DEF_VISIT())

    nbool me::in(const node* elem) const {
        WHEN_NUL(elem).ret(false);
        return in(*elem);
    }

    nbool me::in(const node& elem) const {
        return subs().get([&](const std::string& key, const node& e) { return &e == &elem; });
    }

    tstr<nbicontainer> me::mySubs() const { return subs(); }

    const node* me::deduce(const node& r) const {
        const ntype& ltype = getType();
        const ntype& rtype = r.getType();
        const ntype* res = ltype.deduce(rtype) OR.retNul<node>();
        WHEN(*res == ltype).ret(this);
        WHEN(*res == rtype).ret(&r);

        return nullptr;
    }

    nbool me::canRun(const args& a) const { return prioritize(a) != NO_MATCH; }

    str me::run() { return run(args()); }

    str me::run(const std::string& name) { return run(name, args()); }

    str me::run(const std::string& name, const args& a) {
        if(name.empty()) return run(a);
        node& found = subAll(name, a).get() OR.ret(str());

        return _onRunSub(found, a);
    }

    nbool me::is(const typeProvidable& to) const {
        WHEN_NUL(to).ret(false);
        return is(to.getType());
    }

    nbool me::is(const type& to) const { return getType().is(to); }

    str me::as(const typeProvidable& to) const {
        WHEN_NUL(to).ret(str());
        return as(to.getType());
    }

    str me::as(const type& to) const { return getType().as(*this, to); }

    nbool me::isImpli(const typeProvidable& to) const {
        WHEN_NUL(to).ret(false);
        return isImpli(to.getType());
    }

    nbool me::isImpli(const type& to) const { return getType().isImpli(to); }

    str me::asImpli(const typeProvidable& to) const {
        WHEN_NUL(to).ret(str());
        return asImpli(to.getType());
    }

    str me::asImpli(const type& to) const { return getType().asImpli(*this, to); }

    str me::getEval() const { return str(this); }

    str me::_onRunSub(node& sub, const args& a) { return sub.run(a); }

    nbool me::isComplete() const { return true; }

    void me::inFrame(const bicontainable& args) const {}

    void me::outFrame(const bicontainable& args) const {}

    const modifier& me::getModifier() const {
        static dumModifier inner;
        return inner;
    }

    const src& me::getSrc() const { return dumSrc::singleton(); }

    void me::_setSrc(const src& s) {}
} // namespace nm
