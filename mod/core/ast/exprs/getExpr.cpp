#include "getExpr.hpp"

#include "../../frame/thread.hpp"
#include "../../worker/visitor/graphVisitor.hpp"
#include "../../worker/visitor/visitor.hpp"
#include "../node.inl"
#include "../closure.hpp"

namespace nm {

    NM(DEF_ME(getExpr), DEF_VISIT())

    me::getExpr(const std::string& name): _name(name) {}

    me::getExpr(const std::string& name, const args& a): _name(name), _args(a) {}

    me::getExpr(const node& me, const std::string& name): _me(me), _name(name) {}

    me::getExpr(const node& me, const std::string& name, const args& a):
        _me(me), _name(name), _args(a) {}

    const node& me::getMe() const {
        if(!_me) return thread::get().getNowFrame();
        return *_me;
    }

    str me::getEval() const { return _get(true); }

    str me::run(const args& a) {
        // believe that this expression was checked to be valid.
        return _get(false);
    }

    const std::string& me::getName() const { return _name; }

    void me::setName(const std::string& new1) { _name = new1; }

    /// @return nullable
    const args& me::getArgs() const {
        static args dummy;
        if(!_args.isBind()) return dummy;

        return *_args;
    }

    void me::setArgs(const args& new1) {
        if(new1.len() <= 0) _args.rel();
        else _args.bind(new1);
    }

    str me::_get(nbool evalMode) const {
        NM_DI("getExpr: looking for '%s.%s'", getMe(), _name);
        return _onGet(*_evalMe(evalMode));
    }

    str me::_evalMe(nbool evalMode) const {
        return evalMode ? getMe() THEN(getEval()) : getMe() THEN(template as<node>());
    }

    node& me::_onGet(node& me) const {
        if(nul(me)) return nulOf<node>();

        std::string argsName = _args ? _args->asStr().c_str() : "{}";
        NM_DI("@%s %s.sub(\"%s\", %s)", platformAPI::toAddrId(this), me, _name, argsName);
        if(!_args) return me.sub(_name);

        return me.subAll(_name, *_args).get();
    }

    void me::setMe(const node& newMe) { _me.bind(newMe); }

    void me::onCloneDeep(const clonable& from) {
        me& rhs = (me&) from;
        if(rhs._args) _args.bind((args*) rhs._args->cloneDeep());
    }
} // namespace nm
