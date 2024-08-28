#include "defVarExpr.hpp"

#include "../../frame/thread.hpp"
#include "../../worker/visitor/visitor.hpp"
#include "ast/exprs/assignExpr.hpp"

namespace nm {
    NM(DEF_ME(defVarExpr), DEF_VISIT())

    me::defVarExpr(const std::string& name, const node& rhs): _name(name), _rhs(rhs) {}

    me::defVarExpr(const std::string& name, const node& rhs, const node& to, const src& s,
        const modifier& mod):
        _name(name), _rhs(rhs), _to(to), _src(s), _mod(mod) {}

    str me::run(const args& a) {
        str new1 = _onMakeNew(); // getRight().run();
        if(!new1) return NM_E("new1 is null"), str();

        str to =
            _to ? _to->as<node>() : str(); // here 'as' is mandatory.
                                           // assume that user wrotes 'getExpr("me")' into 'as'.
        scope& s = !to ? thread::get()._getNowFrame().getLocals() : to->subs();
        s.add(_name, *new1);
        return new1;
    }

    const std::string& me::getName() const { return _name; }

    node& me::getRight() { return *_rhs; }

    void me::setRight(const node& rhs) { _rhs.bind(rhs); }

    node& me::getTo() { return *_to; }

    void me::setTo(const node& to) { _to.bind(to); }

    nbool me::isToFrame() const { return !_to; }

    str me::getEval() const { return _rhs->getEval(); }

    clonable* me::cloneDeep() const {
        me* ret = (me*) clone();
        if(_to) ret->_to.bind((node*) _to->cloneDeep());
        if(_rhs) ret->_rhs.bind((node*) _rhs->cloneDeep());

        return ret;
    }

    str me::makeNew() const {
        auto ret = _onMakeNew();
        baseObj* newOrg = (baseObj*) ret->getOrigin().clone();
        if(_src) newOrg->_setSrc(*_src);
        if(_mod) newOrg->_setModifier(*_mod);

        ret->_setOrigin(*newOrg);
        return ret;
    }
} // namespace nm
