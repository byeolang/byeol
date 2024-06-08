#include "frame.hpp"
#include "../ast/obj.hpp"
#include "../ast/baseFunc.hpp"
#include "../visitor/visitor.hpp"
#include "../ast/dumScope.hpp"
#include "thread.hpp"

namespace namu {
    NAMU(DEF_ME(frame), DEF_VISIT())

    me::frame() { _rel(); }
    me::~frame() { _rel(); }

    void me::add(scope& existing) {
        add(nulOf<node>(), existing);
    }
    void me::add(nbicontainer& existing) {
        tstr<scope> wrap = scope::wrap<scope>(existing);
        add(*wrap);
    }
    void me::add(node& owner) {
        add(owner, owner.subs());
    }
    void me::add(node& owner, scope& existing) {
        if(nul(existing)) return;
        if(_stack.size() <= 0)
            return _stack.push_back(scopeRegister{owner, existing}), void();

        tstr<scope> cloned(existing.cloneChain());
        if(!cloned) return;
        cloned->getTail().link(*_getTop().s);
        _stack.push_back(scopeRegister{owner, cloned});
        NAMU_DI("scope added. frames.len[%d] thisFrame.len[%d]", thread::get().getFrames().len(), _stack.size());
    }
    void me::addLocal(const std::string& name, const node& n) {
        if(_stack.size() <= 0)
            return NAMU_E("couldn't push new node. the top scope is null"), void();
        auto& top = _getTop();
        const node& owner = *top.owner;
        if(!nul(owner))
            return NAMU_E("it's tried to add variable into %s. it's not valid.", owner.getType().getName().c_str()), void();

        top.s->add(name, n);
    }

    node& me::getOwner(const node& sub) {
        const nchar* name = sub.getType().getName().c_str();
        for(auto& reg : _stack) {
            nbool isOwner = reg.s->has(sub);
            node& owner = *reg.owner;
            NAMU_DI("sub[%s] is in owner[%s]? == %s", name, nul(owner) ? "null" : owner.getType().getName().c_str(), isOwner ? "true" : "false");
            if(isOwner)
                return *reg.owner;
        }

        NAMU_E("couldn't find owner of %s", sub.getType().getName().c_str());
        return nulOf<node>();
    }

    nbool me::setMe(const baseObj& new1) { return _me.bind(new1); }
    void me::setMe() { _me.rel(); }
    baseObj& me::getMe() { return *_me; }

    void me::del() {
        _stack.pop_back();
        NAMU_DI("scope deleted. frames.len[%d] thisFrame.len[%d]", thread::get().getFrames().len(), _stack.size());
    }

    scopeRegister& me::_getTop() {
        ncnt len = _stack.size();
        if(len <= 0) return nulOf<scopeRegister>();

        return _stack[len-1];
    }

    nbool me::setFunc(baseFunc& new1) { return _func.bind(new1); }
    void me::setFunc() { setFunc(nulOf<baseFunc>()); }
    baseFunc& me::getFunc() { return *_func; }

    // node:
    scope& me::subs() {
        scopeRegister& reg = _getTop();
        static dumScope inner;
        return nul(reg) ? inner : *reg.s;
    }

    priorType me::prioritize(const args& a) const { return NO_MATCH; }

    str me::run(const args& a) { return str(); }

    void me::rel() {
        _rel();
        super::rel();
    }

    const std::vector<scopeRegister>& me::getScopeRegisters() const { return _stack; }

    nbool me::setRet(const node& newRet) const {
        return _ret.bind(newRet);
    }

    nbool me::setRet() const {
        _ret.rel();
        return true;
    }

    node& me::getRet() const { return *_ret; }

    void me::_rel() {
        _me.rel();
        _func.rel();
        _stack.clear();
        _ret.rel();
    }
}
