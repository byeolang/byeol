#include "node.inl"
#include "../builtin/container/native/tnarr.inl"
#include "../type/as.hpp"
#include "args.hpp"
#include "../visitor/visitor.hpp"
#include "src/dumSrc.hpp"

namespace namu {

    template class _nout tnarr<node>;
    template class _nout tucontainable<node>;
    template class _nout tbicontainable<std::string, node>;

    NAMU(DEF_ME(node), DEF_VISIT())

    node& me::operator[](const std::string& name) {
        return sub(name);
    }

    nbool me::has(const node* elem) const {
        return has(*elem);
    }

    nbool me::has(const node& elem) const {
        return !nul(subs().get([&](const std::string& key, const node& e) {
            return &e == &elem;
        }));
    }

    nbool me::canRun(const args& a) const {
        return prioritize(a) != NO_MATCH;
    }

    str me::run() {
        return run(args());
    }

    str me::run(const std::string& name) {
        return run(name, args());
    }

    str me::run(const std::string& name, const args& a) {
        if(name.empty()) return run(a);
        str found = subAll(name, a).getMatch();
        if(!found) return str();

        return _onRunSub(*found, a);
    }

    nbool me::is(const typeProvidable& to) const {
        if(nul(to)) return false;
        return is(to.getType());
    }

    nbool me::is(const type& to) const {
        return getType().is(to);
    }

    str me::as(const typeProvidable& to) const {
        if(nul(to)) return str();
        return as(to.getType());
    }

    str me::as(const type& to) const {
        return getType().as(*this, to);
    }

    nbool me::isImpli(const typeProvidable& to) const {
        if(nul(to)) return false;
        return isImpli(to.getType());
    }

    nbool me::isImpli(const type& to) const {
        return getType().isImpli(to);
    }

    str me::asImpli(const typeProvidable& to) const {
        if(nul(to)) return str();
        return asImpli(to.getType());
    }

    str me::asImpli(const type& to) const {
        return getType().asImpli(*this, to);
    }

    str me::getEval() const {
        return str(this);
    }

    str me::_onRunSub(node& sub, const args& a) {
        return sub.run(a);
    }

    clonable* me::cloneDeep() const {
        return (me*) clone();
    }

    nbool me::isComplete() const {
        return true;
    }

    void me::inFrame(const bicontainable& args) {}

    void me::outFrame() {}

    const src& me::getSrc() const {
        return dumSrc::singletone();
    }

    void me::_setSrc(const src& s) {}
}
