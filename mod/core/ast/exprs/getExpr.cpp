#include "getExpr.hpp"
#include "../../frame/thread.hpp"
#include "../node.inl"
#include "../../visitor/visitor.hpp"
#include "../../visitor/graphVisitor.hpp"

namespace namu {

    NAMU(DEF_ME(getExpr), DEF_VISIT())

    me::getExpr(const std::string& name): _name(name) {}
    me::getExpr(const std::string& name, const args& a): _name(name), _args(a) {}
    me::getExpr(const node& me, const std::string& name): _me(me), _name(name) {}
    me::getExpr(const node& me, const std::string& name, const args& a): _me(me), _name(name), _args(a) {}

    const node& me::getMe() const {
        if(!_me)
            return thread::get().getNowFrame();
        return *_me;
    }

    str me::getEval() const {
        return _get(true).getMatch();
    }

    str me::run(const args& a) {
        // believe that this expression was checked to be valid.
        return _get(false).getMatch();
    }

    const std::string& me::getSubName() const {
        return _name;
    }

    void me::setSubName(const std::string& new1) {
        _name = new1;
    }

    /// @return nullable
    const args& me::getSubArgs() const {
        static args dummy;
        if(!_args.isBind()) return dummy;

        return *_args;
    }

    void me::setSubArgs(const args& new1) {
        if(new1.len() <= 0)
            _args.rel();
        else
            _args.bind(new1);
    }

    priorities me::_get(nbool evalMode) const {
        const node& me = getMe();
        NAMU_DI("getExpr: looking for '%s.%s' evalMode[%s]",
                nul(me) ? "null" : me.getType().getName().c_str(), _name.c_str(), evalMode ? "true" : "false");
        if(nul(me)) return NAMU_E("me == null"), priorities();

        str evalMe = evalMode ? me.getEval() : me.as<node>();
        if(!evalMe) return NAMU_E("me == null"), priorities();
        if(evalMode)
            evalMe = evalMe->as<node>();
        NAMU_DI("run: 'me' was evaluated to %s", evalMe->getType().getName().c_str());

        std::string argsName = _args ? _args->asStr().c_str() : "{}";
        NAMU_DI("run: %s.sub(\"%s\", %s)", evalMe->getType().getName().c_str(), _name.c_str(), argsName.c_str());
        if(!_args)
            return priorities(evalMe->sub(_name));

        return evalMe->subAll(_name, *_args);
    }

    void me::setMe(const node& newMe) {
        _me.bind(newMe);
    }

    clonable* me::cloneDeep() const {
        NAMU_DI("%s.cloneDeep()", getType().getName().c_str());
        me* ret = (me*) clone();
        if(_args) ret->_args.bind((args*) _args->cloneDeep());

        return ret;
    }
}
