#include "defVarExpr.hpp"

#include "../../frame/thread.hpp"
#include "../../worker/visitor/visitor.hpp"
#include "assignExpr.hpp"
#include "../closure.hpp"

namespace nm {
    NM(DEF_ME(defVarExpr), DEF_VISIT())

    me::defVarExpr(const std::string& name, const node& rhs): _name(name), _rhs(rhs) {}

    me::defVarExpr(const std::string& name, const node& rhs, const node& to, const src& s,
        const modifier& mod):
        _name(name), _rhs(rhs), _to(to), _src(s), _mod(mod) {}

    str me::run(const args& a) {
        NM_DI("define variable...");
        str new1 = _makeNew() orRet NM_E("new1 is null"), str();
        // below 'as' is mandatory.
        // assume that user wrotes 'getExpr("me")' into 'as'.
        str to = _to ? _to->as<node>() : str();

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

    str me::getEval() const {
        str ret = _rhs->getEval() orRet ret;
        if(baseFunc::isFuncButNotClosure(*ret)) return closure::make(*ret);
        return ret;
    }

    void me::onCloneDeep(const clonable& from) {
        me& rhs = (me&) from;
        if(rhs._to) _to.bind((node*) rhs._to->cloneDeep());
        if(rhs._rhs) _rhs.bind((node*) rhs._rhs->cloneDeep());
    }

    str me::makeNewOrigin() {
        auto ret = _makeNew();
        baseObj& cast = ret->cast<baseObj>() orRet ret; // `ret` can be a closure

        // origin's clone is making a object, not an origin:
        //  so I've to call cloneDeep().
        baseObj* newOrg = (baseObj*) cast.getOrigin().cloneDeep();
        if(_src) newOrg->_setSrc(*_src);
        if(_mod) newOrg->_setModifier(*_mod);

        cast._setOrigin(*newOrg);
        return ret;
    }

    const modifier& me::getNewModifier() const { return *_mod; }

    str me::_makeNew() {
        str ret = _onMakeNew();
        if(baseFunc::isFuncButNotClosure(*ret)) ret.bind(closure::make(*ret));
        return ret;
    }
} // namespace nm
