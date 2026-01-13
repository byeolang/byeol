#include "core/ast/exprs/getExpr.hpp"

#include "core/frame/thread.hpp"
#include "core/worker/visitor/graphVisitor.hpp"
#include "core/worker/visitor/visitor.hpp"
#include "core/ast/node.inl"

namespace by {

    BY(DEF_ME(getExpr), DEF_VISIT())

    me::getExpr(const std::string& name): _name(name) {}

    me::getExpr(const std::string& name, const args& a): _name(name), _args(a) {}

    me::getExpr(const node& me, const std::string& name): _me(me), _name(name) {}

    me::getExpr(const node& me, const std::string& name, const args& a): _me(me), _name(name), _args(a) {}

    const node* me::getMe() const {
        WHEN(!_me) .ret(thread::get().getNowFrame());
        return _me.get();
    }

    str me::infer() const { return _get(true); }

    str me::eval(const args& a) {
        // believe that this expression was checked to be valid.
        return _get(false);
    }

    const std::string& me::getName() const { return _name; }

    void me::setName(const std::string& new1) { _name = new1; }

    const args& me::getArgs() const {
        static args dummy;
        WHEN(!_args.isBind()) .ret(dummy);

        return *_args;
    }

    void me::setArgs(const args& new1) {
        if(new1.len() <= 0) _args.rel();
        else _args.bind(new1);
    }

    str me::_get(nbool evalMode) const {
        BY_DI("getExpr: looking for '%s.%s'", getMe(), _name);
        return _onGet(_evalMe(evalMode).get());
    }

    str me::_evalMe(nbool evalMode) const { return evalMode ? getMe() TO(infer()) : getMe() TO(template as<node>()); }

    node* me::_onGet(node& me) const {
        std::string argsName = _args ? _args->asStr().c_str() : "{}";
        BY_DI("@%s %s.sub(\"%s\", %s)", this, me, _name, argsName);
        WHEN(!_args) .ret(me.sub(_name));

        return me.subAll(_name, _args.get()).get();
    }

    void me::setMe(const node& newMe) { _me.bind(newMe); }

    void me::onCloneDeep(const clonable& from) {
        me& rhs = (me&) from;
        if(rhs._args) _args.bind((args*) rhs._args->cloneDeep());
    }
} // namespace by
